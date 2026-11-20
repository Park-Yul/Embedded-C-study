/*
 * sr04.c
 *
 *  Created on: 2026. 10. 28.
 *      Author: kccistc
 */

// include
#include "sr04.h"
#include "../../common/delay/delay.h"
#include <stdbool.h>

// define
#define RISE_TIMEOUT_MS		5
#define FALL_TIMEOUT_MS		50

// typedef

// variable declarations

// variable definitions
static GPIO_TypeDef *trig_port;	// C
static uint16_t trig_pin;		// 13
static GPIO_TypeDef *echo_port;	// C
static uint16_t echo_pin;		// 14

static TIM_HandleTypeDef *sr04_htim;	// &htim10

static volatile sr04_state_t state;
static volatile uint32_t rise_cnt;
static volatile uint32_t fall_cnt;

static uint32_t start_ms;

// function declarations
static bool echo_is_high(void);
static uint32_t timer_diff(uint32_t start, uint32_t end);

// function definitions
void sr04_init(GPIO_TypeDef *t_port, uint16_t t_pin, GPIO_TypeDef *e_port, uint16_t e_pin, TIM_HandleTypeDef *htim){
	trig_port = t_port;
	trig_pin = t_pin;
	echo_port = e_port;
	echo_pin = e_pin;
	sr04_htim = htim;

	delay_init();
	HAL_GPIO_WritePin(trig_port, trig_pin, GPIO_PIN_RESET);
	HAL_TIM_Base_Start(sr04_htim);
	state = STATE_IDLE;
}

sr04_status_t sr04_start(void){
	if(state == STATE_WAIT_RISE || state == STATE_WAIT_FALL){
		return SR04_ERR_BUSY;
	}
	if(echo_is_high()){
		return SR04_ERR_BUSY;
	}
	start_ms = HAL_GetTick();
	state = STATE_WAIT_RISE;

	HAL_GPIO_WritePin(trig_port, trig_pin, GPIO_PIN_SET);
	delay_us(10);
	HAL_GPIO_WritePin(trig_port, trig_pin, GPIO_PIN_RESET);
	return SR04_IN_PROGRESS;
}

static bool echo_is_high(void){
	return (HAL_GPIO_ReadPin(echo_port, echo_pin) == GPIO_PIN_SET);
}

void sr04_echoISR(void){
	uint32_t cnt = __HAL_TIM_GET_COUNTER(sr04_htim);
	bool high = echo_is_high();
	if((state == STATE_WAIT_RISE) && high) {
		rise_cnt = cnt;
		state = STATE_WAIT_FALL;
	}
	else if((state == STATE_WAIT_FALL) && !high){
		fall_cnt = cnt;
		state = STATE_DONE;
	}
}

static uint32_t timer_diff(uint32_t start, uint32_t end){
	uint32_t arr = __HAL_TIM_GET_AUTORELOAD(sr04_htim);
	if(end >= start){
		return end - start;
	}
	else{
		return (arr - start) + end + 1;
	}
}

sr04_status_t sr04_getResult(uint32_t *echo_us){
	uint32_t elapsed_ms = HAL_GetTick() - start_ms;
	switch(state){
	case STATE_WAIT_RISE:
		if(elapsed_ms <= RISE_TIMEOUT_MS){
			return SR04_IN_PROGRESS;
		}
		state = STATE_IDLE;
		return SR04_ERR_NO_ECHO;
		break;
	case STATE_WAIT_FALL:
		if(elapsed_ms <= FALL_TIMEOUT_MS){
			return SR04_IN_PROGRESS;
		}
		state = STATE_IDLE;
		return SR04_ERR_NO_ECHO;
	case STATE_DONE:
		uint32_t us = timer_diff(rise_cnt, fall_cnt);
		state = STATE_IDLE;
		*echo_us = us;
		return SR04_OK;
	case STATE_IDLE:
		return SR04_ERR_NOT_STARTED;
	default:
		return SR04_ERR_NOT_STARTED;
	}
}

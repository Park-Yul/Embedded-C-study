/*
 * distance.c
 *
 *  Created on: 2026. 10. 29.
 *      Author: kccistc
 */

// include
#include "distance.h"
#include "../../../driver/sr04/sr04.h"

// define

// typedef

// variable declarations
extern TIM_HandleTypeDef htim10;

// variable definitions
static uint32_t echo_us = 0;
static bool start_flag = false;
static sr04_status_t sr04_status = SR04_ERR_NOT_STARTED;
static uint32_t request_ms;

// function declarations

// function definitions
void distance_init(void){
	sr04_init(GPIOC, GPIO_PIN_13, GPIOC, GPIO_PIN_14, &htim10);
	echo_us = 0;
	start_flag = false;
	sr04_status = SR04_ERR_NOT_STARTED;
	request_ms = HAL_GetTick();
}


void distance_task(void){
	if(start_flag && sr04_status == SR04_IN_PROGRESS){
		sr04_status = sr04_getResult(&echo_us);
		if(sr04_status != SR04_IN_PROGRESS && sr04_status != SR04_OK){
			start_flag = false;
		}
	}
	if(!start_flag && HAL_GetTick() - request_ms >= 1000U){
		distance_request();
	}
}

void distance_request(void){
	if(start_flag){
		return;
	}
	request_ms = HAL_GetTick();
	sr04_status_t state_start = sr04_start();
	sr04_status = state_start;
	if(state_start == SR04_IN_PROGRESS){
		start_flag = true;
	}
}

distance_status_t distance_get(uint16_t *distance_cm){
	if(sr04_status == SR04_ERR_NO_ECHO){
		sr04_status = SR04_ERR_NOT_STARTED;
		return DISTANCE_NO_ECHO;
	}
	if(start_flag != true){
		return DISTANCE_NOT_READY;
	}
	if(sr04_status != SR04_OK){
		return DISTANCE_IN_PROGRESS;
	}
	start_flag = false;
	sr04_status = SR04_ERR_NOT_STARTED;
	*distance_cm = (echo_us * 17)  / 1000;
	return DISTANCE_OK;
}

void distance_ISR(void){
	sr04_echoISR();
}

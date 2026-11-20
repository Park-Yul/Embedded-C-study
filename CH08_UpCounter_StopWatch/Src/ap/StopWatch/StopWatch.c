/*
 * StopWatch.c
 *
 *  Created on: 2026. 10. 16.
 *      Author: kccistc
 */

#include "../../driver/led/led.h"
#include "../../driver/button/button.h"
#include "../../driver/fnd/fnd.h"
#include "StopWatch.h"

typedef enum{
	STOP,
	RUN,
	CLEAR
} StopWatch_t;

static StopWatch_t StopWatch_state;
static uint16_t counter;

static led_t led_run;
static led_t led_stop;

static button_t btn_run_stop;
static button_t btn_clear;

extern TIM_HandleTypeDef htim3;

static void StopWatch_setState(StopWatch_t s);
static StopWatch_t StopWatch_getState(void);
static void StopWatch_incCounter(void);
static void StopWatch_clearCounter(void);
static void StopWatch_setCounter(uint16_t c);
static uint16_t StopWatch_getCounter(void);
static uint16_t StopWatch_getCounter(void);
static void StopWatch_stop(void);
static void StopWatch_run(void);
static void StopWatch_clear(void);

//button_t btn_mode = {GPIOC, GPIO_PIN_3, RELEASED};


void StopWatch_init(void)
{
	StopWatch_state = STOP;
	counter = 0;

	led_init(&led_stop, GPIOB, GPIO_PIN_13);
	led_init(&led_run, GPIOC, GPIO_PIN_4);

	button_init(&btn_run_stop, GPIOC, GPIO_PIN_0);
	button_init(&btn_clear, GPIOC, GPIO_PIN_1);

	HAL_TIM_Base_Start_IT(&htim3);
}

void StopWatch_task(void)
{
	switch(StopWatch_state)
	{
	case STOP:
		StopWatch_stop();
		break;
	case RUN:
		StopWatch_run();
		break;
	case CLEAR:
		StopWatch_clear();
		break;
	default:
		StopWatch_stop();
		break;
	}
}


static void StopWatch_setState(StopWatch_t s)
{
	StopWatch_state = s;
}

static StopWatch_t StopWatch_getState(void)
{
	return StopWatch_state;
}


static void StopWatch_incCounter(void)
{
	counter++;
}

static void StopWatch_clearCounter(void)
{
	counter = 0;
}

static void StopWatch_setCounter(uint16_t c)
{
	counter = c;
}

static uint16_t StopWatch_getCounter(void)
{
	return counter;
}


static void StopWatch_stop(void)
{
	if(button_getState(&btn_run_stop) == ACT_PUSH){
		StopWatch_setState(RUN);
	}
	else if (button_getState(&btn_clear) == ACT_PUSH){
		StopWatch_setState(CLEAR);
	}
	uint16_t num = StopWatch_getCounter();
	fnd_set_num(num);

	led_on(&led_stop);
	led_off(&led_run);
}

static void StopWatch_run(void)
{
	if (button_getState(&btn_run_stop) == ACT_PUSH){
		StopWatch_setState(STOP);
	}

	uint16_t num = StopWatch_getCounter();

	static uint32_t prevTime = 0;
	uint32_t curTime = HAL_GetTick();
	if(curTime - prevTime < 99) {
		fnd_set_num(num);
//		fnd_show_number(num);
		return;
	}
	prevTime = curTime;

	if (num == 9999){
		StopWatch_clearCounter();
	}
	else {
		StopWatch_incCounter();
	}
	num =  StopWatch_getCounter();
	fnd_set_num(num);

	led_off(&led_stop);
	led_on(&led_run);

}

static void StopWatch_clear(void)
{
	uint16_t num;

	StopWatch_clearCounter();
	num = StopWatch_getCounter();

//	fnd_show_number(num);
	fnd_set_num(num);
	StopWatch_setState(STOP);
}

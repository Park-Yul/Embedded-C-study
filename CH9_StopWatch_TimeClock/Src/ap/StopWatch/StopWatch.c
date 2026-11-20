/*
 * StopWatch.c
 *
 *  Created on: 2026. 10. 16.
 *      Author: kccistc
 */
#include "StopWatch.h"
#include "../../driver/led/led.h"
#include "../../driver/button/button.h"
#include "../../driver/fnd/fnd.h"

extern TIM_HandleTypeDef htim3;

static StopWatch_t StopWatchData;

static StopWatch_state_t StopWatch_state;
static uint16_t counter;

static led_t led_stop;
static led_t led_run;

static button_t btn_run_stop;
static button_t btn_clear;



static void StopWatch_setState(StopWatch_state_t s);
//static StopWatch_state_t StopWatch_getState(void);
static void StopWatch_incCounter(void);
static void StopWatch_clearCounter(void);
//static void StopWatch_setCounter(uint16_t c);
static uint16_t StopWatch_getCounter(void);
static void StopWatch_stop(void);
static void StopWatch_run(void);
static void StopWatch_clear(void);
static void StopWatch_dispTime(void);


void StopWatch_init(void)
{
	StopWatch_state = STOP;
	counter = 0;

	led_init(&led_stop, GPIOB, GPIO_PIN_13);
	led_init(&led_run, GPIOC, GPIO_PIN_4);

	button_init(&btn_run_stop, GPIOC, GPIO_PIN_0);
	button_init(&btn_clear, GPIOC, GPIO_PIN_1);

	StopWatchData.hour = 0;
	StopWatchData.min = 0;
	StopWatchData.sec = 0;
	StopWatchData.msec = 0;

	HAL_TIM_Base_Start_IT(&htim3);
}

void StopWatch_ISR(void){
	if(StopWatch_state == RUN){
		StopWatch_incCounter();
	}
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


static void StopWatch_setState(StopWatch_state_t s)
{
	StopWatch_state = s;
}

//static StopWatch_state_t StopWatch_getState(void)
//{
//	return StopWatch_state;
//}


static void StopWatch_incCounter(void)
{
	if (StopWatchData.msec < 99) {
		StopWatchData.msec++;
		return;
	}
	StopWatchData.msec = 0;
	if(StopWatchData.sec < 59) {
		StopWatchData.sec++;
		return;
	}
	StopWatchData.sec = 0;
	if(StopWatchData.min < 59) {
		StopWatchData.min++;
		return;
	}
	StopWatchData.min = 0;
	if(StopWatchData.hour < 23) {
		StopWatchData.hour++;
		return;
	}
	StopWatchData.hour = 0;
}

static void StopWatch_clearCounter(void)
{
	StopWatchData.hour = 0;
	StopWatchData.min = 0;
	StopWatchData.sec = 0;
	StopWatchData.msec = 0;
}

//static void StopWatch_setCounter(uint16_t c)
//{
//	counter = c;
//}

static uint16_t StopWatch_getCounter(void)
{
	return (StopWatchData.sec * 100) + StopWatchData.msec;
}


static void StopWatch_stop(void)
{
	if(button_getState(&btn_run_stop) == ACT_PUSH){
		StopWatch_setState(RUN);
	}
	else if (button_getState(&btn_clear) == ACT_PUSH){
		StopWatch_setState(CLEAR);
	}
	StopWatch_dispTime();
	led_on(&led_stop);
	led_off(&led_run);
}

static void StopWatch_run(void)
{
	if (button_getState(&btn_run_stop) == ACT_PUSH){
		StopWatch_setState(STOP);
	}
	StopWatch_dispTime();
	led_off(&led_stop);
	led_on(&led_run);
}

static void StopWatch_clear(void)
{
	StopWatch_clearCounter();
	StopWatch_dispTime();
	StopWatch_setState(STOP);
}

static void StopWatch_dispTime(void) {
	uint16_t num;

	num = StopWatch_getCounter();
	fnd_set_num(num);

	if ((StopWatchData.msec) < 50) {
		fnd_set_dp(FND_DIGIT_2, FND_DP_ON);
	}
	else {
		fnd_set_dp(FND_DIGIT_2, FND_DP_OFF);
	}
}

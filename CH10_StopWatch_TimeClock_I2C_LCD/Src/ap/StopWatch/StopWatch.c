/*
 * StopWatch.c
 *
 *  Created on: 2026. 10. 16.
 *      Author: kccistc
 */
#include "StopWatch.h"
//#include "../../driver/led/led.h"
#include "../../driver/button/button.h"
//#include "../../driver/fnd/fnd.h"

// variables
static StopWatch_t StopWatchData;
static StopWatch_state_t StopWatch_state;
static button_t btn_run_stop;
static button_t btn_clear;

// function prototypes
void StopWatch_init(void);
void StopWatch_task(void);
StopWatch_t *StopWatch_getTimeData(void);
StopWatch_state_t StopWatch_getState(void);
void StopWatch_ISR(void);
static void StopWatch_setState(StopWatch_state_t s);
static void StopWatch_incTime(void);
static void StopWatch_clearTime(void);
static void StopWatch_stop(void);
static void StopWatch_run(void);
static void StopWatch_clear(void);

// function definitions
void StopWatch_init(void)
{
	StopWatch_state = STOP;
	button_init(&btn_run_stop, GPIOC, GPIO_PIN_0);
	button_init(&btn_clear, GPIOC, GPIO_PIN_1);
	StopWatchData.hour = 0;
	StopWatchData.min = 0;
	StopWatchData.sec = 0;
	StopWatchData.msec = 0;
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

StopWatch_t *StopWatch_getTimeData(void){
	return &StopWatchData;
}

StopWatch_state_t StopWatch_getState(void){
	return StopWatch_state;
}

void StopWatch_ISR(void){
	if(StopWatch_state == RUN){
		StopWatch_incTime();
	}
}

static void StopWatch_setState(StopWatch_state_t s)
{
	StopWatch_state = s;
}


static void StopWatch_incTime(void)
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

static void StopWatch_clearTime(void)
{
	StopWatchData.hour = 0;
	StopWatchData.min = 0;
	StopWatchData.sec = 0;
	StopWatchData.msec = 0;
}

static void StopWatch_stop(void)
{
	if(button_getState(&btn_run_stop) == ACT_PUSH){
		StopWatch_setState(RUN);
	}
	else if (button_getState(&btn_clear) == ACT_PUSH){
		StopWatch_setState(CLEAR);
	}
}

static void StopWatch_run(void)
{
	if (button_getState(&btn_run_stop) == ACT_PUSH){
		StopWatch_setState(STOP);
	}

}

static void StopWatch_clear(void)
{
	StopWatch_clearTime();
	StopWatch_setState(STOP);
}

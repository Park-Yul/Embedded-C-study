/*
 * StopWatch.c
 *
 *  Created on: 2026. 10. 16.
 *      Author: kccistc
 */

// include
#include "StopWatch.h"
#include "../../../driver/button/button.h"
#include "../../../common/queue/que_buffer.h"

// define

// typedef

// variables declarations
extern que_buf_t que_btnEvent;
extern btn_event_t btnEvent[BTN_EVENT_Q_SIZE];

// variables definitions
bool runstop_Flag = false;
bool clear_Flag = false;
static StopWatch_state_t StopWatch_state;
static StopWatch_t StopWatchData;

// function declarations
static void StopWatch_incTime(void);
static void StopWatch_clearTime(void);
static void StopWatch_stop(void);
static void StopWatch_run(void);
static void StopWatch_clear(void);

// function definitions
void StopWatch_init(void)
{
	StopWatchData.hour = 0;
	StopWatchData.min = 0;
	StopWatchData.sec = 0;
	StopWatchData.msec = 0;
	StopWatch_state = STOP;
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

void StopWatch_setState(StopWatch_state_t s)
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
	if(runstop_Flag == true){
		StopWatch_setState(RUN);
	}
	if(clear_Flag == true){
		StopWatch_setState(CLEAR);
	}
}

static void StopWatch_run(void)
{
	if(runstop_Flag == true){
		StopWatch_setState(STOP);
	}
}

static void StopWatch_clear(void)
{
	StopWatch_clearTime();
	StopWatch_setState(STOP);
}

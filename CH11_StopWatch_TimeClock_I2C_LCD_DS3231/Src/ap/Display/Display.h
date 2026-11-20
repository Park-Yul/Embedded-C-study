/*
 * Display.h
 *
 *  Created on: 2026. 10. 20.
 *      Author: kccistc
 */

#ifndef SRC_AP_DISPLAY_DISPLAY_H_
#define SRC_AP_DISPLAY_DISPLAY_H_

// include
#include <string.h>
#include "stm32f4xx_hal.h"

// define

// typedef
typedef struct{
	uint8_t hour;
	uint8_t min;
	uint8_t sec;
	uint8_t msec;
} TimeClockTime_t;

typedef struct{
	uint8_t day;
	uint8_t date;
	uint8_t month;
	uint8_t year;
} TimeClockDate_t;

typedef struct{
	uint8_t hour;
	uint8_t min;
	uint8_t sec;
	uint8_t msec;
} StopWatchTime_t;

enum {
	TIMECLOCK_MODE,
	STOPWATCH_MODE
};

enum {
	STOPWATCH_STOP,
	STOPWATCH_RUN
};

// function prototypes
void Disp_init(void);
void Disp_setMode(int mode);
void Disp_setStopWatchState(int state);
void Disp_setTimeClockTime(TimeClockTime_t *pTime);
void Disp_setTimeClockDate(TimeClockDate_t *pDate);
void Disp_setStopWatchTime(StopWatchTime_t *pTime);
void Disp_ISR(void);

#endif /* SRC_AP_DISPLAY_DISPLAY_H_ */

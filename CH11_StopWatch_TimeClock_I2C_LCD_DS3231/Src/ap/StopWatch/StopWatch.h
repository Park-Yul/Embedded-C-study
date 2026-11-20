/*
 * StopWatch.h
 *
 *  Created on: 2026. 10. 16.
 *      Author: kccistc
 */

#ifndef SRC_AP_STOPWATCH_STOPWATCH_H_
#define SRC_AP_STOPWATCH_STOPWATCH_H_

// include
#include "stm32f4xx_hal.h"

// define

// typedef
typedef struct{
	uint8_t hour;
	uint8_t min;
	uint8_t sec;
	uint8_t msec;
} StopWatch_t;

typedef enum{
	STOP,
	RUN,
	CLEAR
} StopWatch_state_t;

// function prototypes
void StopWatch_init(void);
void StopWatch_task(void);
StopWatch_t * StopWatch_getTimeData(void);
StopWatch_state_t StopWatch_getState(void);
void StopWatch_ISR(void);

#endif /* SRC_AP_STOPWATCH_STOPWATCH_H_ */

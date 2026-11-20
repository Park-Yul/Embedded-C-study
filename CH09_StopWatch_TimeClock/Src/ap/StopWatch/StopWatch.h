/*
 * StopWatch.h
 *
 *  Created on: 2026. 10. 16.
 *      Author: kccistc
 */

//#include "stm32f4xx_hal.h"

#ifndef SRC_AP_STOPWATCH_STOPWATCH_H_
#define SRC_AP_STOPWATCH_STOPWATCH_H_
#include "stm32f4xx_hal.h"

typedef enum{
	STOP,
	RUN,
	CLEAR
} StopWatch_state_t;

typedef struct{
	uint8_t hour;
	uint8_t min;
	uint8_t sec;
	uint8_t msec;
} StopWatch_t;

void StopWatch_init(void);
void StopWatch_ISR(void);
void StopWatch_task(void);


#endif /* SRC_AP_STOPWATCH_STOPWATCH_H_ */

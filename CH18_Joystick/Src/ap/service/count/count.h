/*
 * count.h
 *
 *  Created on: 2026. 11. 3.
 *      Author: kccistc
 */

#ifndef SRC_AP_SERVICE_COUNT_COUNT_H_
#define SRC_AP_SERVICE_COUNT_COUNT_H_

// include
#include "stm32f4xx_hal.h"

// define

// typedef

// variable declarations

// function declarations
void count_init(void);
void count_task(void);
int32_t getXcount(void);
int32_t getYcount(void);
void count_ISR(void);

#endif /* SRC_AP_SERVICE_COUNT_COUNT_H_ */

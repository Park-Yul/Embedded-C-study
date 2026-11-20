/*
 * display.h
 *
 *  Created on: 2026. 10. 28.
 *      Author: kccistc
 */

#ifndef SRC_AP_SERVICE_DISPLAY_DISPLAY_H_
#define SRC_AP_SERVICE_DISPLAY_DISPLAY_H_

// include

// define
#include "stm32f4xx_hal.h"

// typedef

// variable declarations

// function declarations
void display_init(void);
void display_THtask(void);
void display_distanceTask(void);

#endif /* SRC_AP_SERVICE_DISPLAY_DISPLAY_H_ */

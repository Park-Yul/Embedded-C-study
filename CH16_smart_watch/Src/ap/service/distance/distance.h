/*
 * distance.h
 *
 *  Created on: 2026. 10. 29.
 *      Author: kccistc
 */

#ifndef SRC_AP_SERVICE_DISTANCE_DISTANCE_H_
#define SRC_AP_SERVICE_DISTANCE_DISTANCE_H_

// include
#include "stm32f4xx_hal.h"
#include <stdbool.h>


// define

// typedef
typedef enum{
	DISTANCE_NOT_READY,
	DISTANCE_IN_PROGRESS,
	DISTANCE_OK,
	DISTANCE_NO_ECHO
}distance_status_t;

// variable declarations

// function declarations

void distance_init(void);
void distance_task(void);
void distance_request(void);
distance_status_t distance_get(uint16_t *distance_cm);
void distance_ISR(void);

#endif /* SRC_AP_SERVICE_DISTANCE_DISTANCE_H_ */

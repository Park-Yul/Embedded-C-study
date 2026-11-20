/*
 * joystick.h
 *
 *  Created on: 2026. 11. 3.
 *      Author: kccistc
 */

#ifndef SRC_DRIVER_JOYSTICK_JOYSTICK_H_
#define SRC_DRIVER_JOYSTICK_JOYSTICK_H_

// include
#include "stm32f4xx_hal.h"
#include <stdio.h>

// define

// typedef
typedef struct
{
	uint16_t x;
	uint16_t y;
}joystick_data_t;


// variable declarations

// function declarations
void joystick_init(ADC_HandleTypeDef *hadc);
HAL_StatusTypeDef joystick_read(joystick_data_t *data);
void joystic_print(void);
void joystick_ISR(void);

#endif /* SRC_DRIVER_JOYSTICK_JOYSTICK_H_ */

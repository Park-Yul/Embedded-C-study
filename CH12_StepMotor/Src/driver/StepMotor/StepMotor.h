/*
 * StepMotor.h
 *
 *  Created on: 2026. 10. 23.
 *      Author: kccistc
 */

#ifndef SRC_DRIVER_STEPMOTOR_STEPMOTOR_H_
#define SRC_DRIVER_STEPMOTOR_STEPMOTOR_H_

// include
#include "stm32f4xx_hal.h"

// define
#define STEP_COUNT	8

// typedef
typedef struct {
	GPIO_TypeDef* port[4];
	uint16_t pin[4];
} StepMotor_pin_t;

typedef struct{
	StepMotor_pin_t *pin;
	uint8_t	stepCounter;
}StepMotor_t;

typedef enum {
	STEP_MOTOR_DIR_CW,
	STEP_MOTOR_DIR_CCW,
}StepMotor_dir_e;

// variables

// function prototypes
void StepMotor_StepPhase(StepMotor_t *m, StepMotor_dir_e dir);
void StepMotor_release(StepMotor_t *m);


#endif /* SRC_DRIVER_STEPMOTOR_STEPMOTOR_H_ */

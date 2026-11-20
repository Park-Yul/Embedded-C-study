/*
 * StepMotor.c
 *
 *  Created on: 2026. 10. 23.
 *      Author: kccistc
 */

// include
#include "StepMotor.h"

// variables
uint8_t step_table[STEP_COUNT] = {0x01, 0x03, 0x02, 0x06, 0x04, 0x0C, 0x08, 0x09};

// function prototypes
static void StepMotor_WritePattern(StepMotor_t *m, uint8_t step);

// function definitions
static void StepMotor_WritePattern(StepMotor_t *m, uint8_t step){
	for(int i=0 ; i<4 ; i++){
		GPIO_PinState s = (step & (1<<i)) ? GPIO_PIN_SET : GPIO_PIN_RESET;
		HAL_GPIO_WritePin(m->pin->port[i], m->pin->pin[i], s);
	}
}

void StepMotor_StepPhase(StepMotor_t *m, StepMotor_dir_e dir){
	if(dir == STEP_MOTOR_DIR_CW){
		m->stepCounter = (m->stepCounter + STEP_COUNT - 1) % STEP_COUNT;
	}
	else {
		m->stepCounter = (m->stepCounter + 1) % STEP_COUNT;
	}
	StepMotor_WritePattern(m, step_table[m->stepCounter]);
}

void StepMotor_release(StepMotor_t *m){
	StepMotor_WritePattern(m, 0x00);
}

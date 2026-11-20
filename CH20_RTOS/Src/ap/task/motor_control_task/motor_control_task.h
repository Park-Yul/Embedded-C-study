/*
 * motor_control_task.h
 *
 *  Created on: 2026. 11. 12.
 *      Author: kccistc
 */

#ifndef SRC_AP_TASK_MOTOR_CONTROL_TASK_MOTOR_CONTROL_TASK_H_
#define SRC_AP_TASK_MOTOR_CONTROL_TASK_MOTOR_CONTROL_TASK_H_

/* include */
#include "stm32f4xx_hal.h"

/* define */

/* typedef */
typedef enum{
	STOP,
	RUN,
	RIGHT,
	LEFT
} motor_state_t;

/* variable declarations */
extern motor_state_t motor_state;

/* function declarations */
void motor_control_task_init(void);
void motor_control_task(void);

#endif /* SRC_AP_TASK_MOTOR_CONTROL_TASK_MOTOR_CONTROL_TASK_H_ */

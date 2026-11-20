/*
 * dc_motor.h
 *
 *  Created on: 2026. 11. 11.
 *      Author: kccistc
 */

#ifndef SRC_DRIVER_DC_MOTOR_DC_MOTOR_H_
#define SRC_DRIVER_DC_MOTOR_DC_MOTOR_H_

/* include */
#include "stm32f4xx_hal.h"

/* define */
typedef struct{
	TIM_HandleTypeDef *htim;
	uint32_t Channel;
	GPIO_TypeDef *GPIO1;
	uint16_t GPIO1_Pin;
	GPIO_TypeDef *GPIO2;
	uint16_t GPIO2_Pin;
}motor_t;

/* typedef */

/* variable declarations */

/* function declarations */
void motor_init(motor_t *motor, TIM_HandleTypeDef *htim, uint32_t Channel, GPIO_TypeDef *GPIO1, uint16_t GPIO1_Pin, GPIO_TypeDef *GPIO2, uint16_t GPIO2_Pin);
void motor_set_speed(int16_t speed);
void motor_stop(motor_t *motor_right, motor_t *motor_left);
void motor_forward(motor_t *motor_right, motor_t *motor_left);
void motor_backward(motor_t *motor_right, motor_t *motor_left);
void motor_right(motor_t *motor_right, motor_t *motor_left);
void motor_left(motor_t *motor_right, motor_t *motor_left);

#endif /* SRC_DRIVER_DC_MOTOR_DC_MOTOR_H_ */

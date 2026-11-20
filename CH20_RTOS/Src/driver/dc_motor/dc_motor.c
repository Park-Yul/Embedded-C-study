/*
 * dc_motor.c
 *
 *  Created on: 2026. 11. 11.
 *      Author: kccistc
 */


/* include */
#include "dc_motor.h"

/* define */

/* typedef */

/* variable declarations */

/* variable definitions */
static int16_t motor_speed;

/* function declarations */

/* function definitions */
void motor_init(motor_t *motor, TIM_HandleTypeDef *htim, uint32_t Channel, GPIO_TypeDef *GPIO1, uint16_t GPIO1_Pin, GPIO_TypeDef *GPIO2, uint16_t GPIO2_Pin){
	motor->htim = htim;
	motor->Channel = Channel;
	motor->GPIO1 = GPIO1;
	motor->GPIO1_Pin = GPIO1_Pin;
	motor->GPIO2 = GPIO2;
	motor->GPIO2_Pin = GPIO2_Pin;
}

void motor_set_speed(int16_t speed){
	if(speed > 1000){
		speed = 1000;
	}
	else if(speed <0){
		speed = 1;
	}
	motor_speed = speed;
}

void motor_stop(motor_t *motor_right, motor_t *motor_left){
	HAL_TIM_PWM_Stop(motor_right->htim, motor_right->Channel);
	HAL_TIM_PWM_Stop(motor_left->htim, motor_left->Channel);
}


void motor_forward(motor_t *motor_right, motor_t *motor_left){
	__HAL_TIM_SET_COMPARE(motor_right->htim, motor_right->Channel, motor_speed);
	HAL_GPIO_WritePin(motor_right->GPIO1, motor_right->GPIO1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(motor_right->GPIO2, motor_right->GPIO2_Pin, GPIO_PIN_SET);
	// (PB6, PA7) = (RESET, SET) -> right forward
	// (PB6, PA7) = (SET, RESET) -> right reverse
	// (PB6, PA7) = (RESET, RESET) -> right stop

	__HAL_TIM_SET_COMPARE(motor_left->htim, motor_left->Channel, motor_speed);
	HAL_GPIO_WritePin(motor_left->GPIO1, motor_left->GPIO1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(motor_left->GPIO2, motor_left->GPIO2_Pin, GPIO_PIN_SET);
	// (PA6, PA5) = (RESET, SET) -> left forward
	// (PA6, PA5) = (SET, RESET) -> left reverse
	// (PA6, PA5) = (RESET, RESET) -> left stop

	HAL_TIM_PWM_Start(motor_right->htim, motor_right->Channel);
	HAL_TIM_PWM_Start(motor_left->htim, motor_left->Channel);
}

void motor_backward(motor_t *motor_right, motor_t *motor_left){
	__HAL_TIM_SET_COMPARE(motor_right->htim, motor_right->Channel, motor_speed);
	HAL_GPIO_WritePin(motor_right->GPIO1, motor_right->GPIO1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(motor_right->GPIO2, motor_right->GPIO2_Pin, GPIO_PIN_RESET);
	// (PB6, PA7) = (RESET, SET) -> right forward
	// (PB6, PA7) = (SET, RESET) -> right reverse
	// (PB6, PA7) = (RESET, RESET) -> right stop

	__HAL_TIM_SET_COMPARE(motor_left->htim, motor_left->Channel, motor_speed);
	HAL_GPIO_WritePin(motor_left->GPIO1, motor_left->GPIO1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(motor_left->GPIO2, motor_left->GPIO2_Pin, GPIO_PIN_RESET);
	// (PA6, PA5) = (RESET, SET) -> left forward
	// (PA6, PA5) = (SET, RESET) -> left reverse
	// (PA6, PA5) = (RESET, RESET) -> left stop

	HAL_TIM_PWM_Start(motor_right->htim, motor_right->Channel);
	HAL_TIM_PWM_Start(motor_left->htim, motor_left->Channel);
}

void motor_right(motor_t *motor_right, motor_t *motor_left){
	__HAL_TIM_SET_COMPARE(motor_right->htim, motor_right->Channel, motor_speed);
	HAL_GPIO_WritePin(motor_right->GPIO1, motor_right->GPIO1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(motor_right->GPIO2, motor_right->GPIO2_Pin, GPIO_PIN_RESET);
	// (PB6, PA7) = (RESET, SET) -> right forward
	// (PB6, PA7) = (SET, RESET) -> right reverse
	// (PB6, PA7) = (RESET, RESET) -> right stop

																																																																																																																																																									__HAL_TIM_SET_COMPARE(motor_left->htim, motor_left->Channel, motor_speed);
	HAL_GPIO_WritePin(motor_left->GPIO1, motor_left->GPIO1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(motor_left->GPIO2, motor_left->GPIO2_Pin, GPIO_PIN_SET);
	// (PA6, PA5) = (RESET, SET) -> left forward
	// (PA6, PA5) = (SET, RESET) -> left reverse
	// (PA6, PA5) = (RESET, RESET) -> left stop

	HAL_TIM_PWM_Start(motor_right->htim, motor_right->Channel);
	HAL_TIM_PWM_Start(motor_left->htim, motor_left->Channel);
}

void motor_left(motor_t *motor_right, motor_t *motor_left){
	__HAL_TIM_SET_COMPARE(motor_right->htim, motor_right->Channel, motor_speed);
	HAL_GPIO_WritePin(motor_right->GPIO1, motor_right->GPIO1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(motor_right->GPIO2, motor_right->GPIO2_Pin, GPIO_PIN_SET);
	// (PB6, PA7) = (RESET, SET) -> right forward
	// (PB6, PA7) = (SET, RESET) -> right reverse
	// (PB6, PA7) = (RESET, RESET) -> right stop

	__HAL_TIM_SET_COMPARE(motor_left->htim, motor_left->Channel, motor_speed);
	HAL_GPIO_WritePin(motor_left->GPIO1, motor_left->GPIO1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(motor_left->GPIO2, motor_left->GPIO2_Pin, GPIO_PIN_RESET);
	// (PA6, PA5) = (RESET, SET) -> left forward
	// (PA6, PA5) = (SET, RESET) -> left reverse
	// (PA6, PA5) = (RESET, RESET) -> left stop

	HAL_TIM_PWM_Start(motor_right->htim, motor_right->Channel);
	HAL_TIM_PWM_Start(motor_left->htim, motor_left->Channel);
}

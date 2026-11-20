/*
 * sr04.c.h
 *
 *  Created on: 2026. 10. 28.
 *      Author: kccistc
 */

#ifndef SRC_DRIVER_SR04_SR04_H_
#define SRC_DRIVER_SR04_SR04_H_

// include
#include "stm32f4xx_hal.h"
#include <stdbool.h>

// define

// typedef
typedef enum{
	STATE_IDLE,
	STATE_WAIT_RISE,
	STATE_WAIT_FALL,
	STATE_DONE
}sr04_state_t;

typedef enum{
	SR04_OK,
	SR04_IN_PROGRESS,
	SR04_ERR_NOT_STARTED,
	SR04_ERR_BUSY,
	SR04_ERR_NO_ECHO,
	SR04_ERR_OUT_OF_RANGE
}sr04_status_t;

// variable definitions

// function definitions

void sr04_init(GPIO_TypeDef *t_port, uint16_t t_pin, GPIO_TypeDef *e_port, uint16_t e_pin, TIM_HandleTypeDef *htim);
sr04_status_t sr04_start(void);
void sr04_echoISR(void);
sr04_status_t sr04_getResult(uint32_t *echo_us);


#endif /* SRC_DRIVER_SR04_SR04_H_ */

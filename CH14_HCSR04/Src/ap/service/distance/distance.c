/*
 * distance.c
 *
 *  Created on: 2026. 10. 29.
 *      Author: kccistc
 */


// include
#include "distance.h"
#include "../../../driver/sr04/sr04.h"
#include <stdbool.h>

// define

// typedef

// variable declarations
extern TIM_HandleTypeDef htim10;

// variable definitions
uint32_t echo_us = 0;
bool start_flag = false;
sr04_status_t sr04_status;

// function declarations

// function definitions
void distance_init(void){
	sr04_init(GPIOC, GPIO_PIN_13, GPIOC, GPIO_PIN_14, &htim10);
}


void distance_task(void){
	if(sr04_getResult(&echo_us) == SR04_OK){
		sr04_status = SR04_OK;
	}
}

void distance_request(void){
	start_flag = true;
	sr04_start();
}

distance_status_t distance_get(uint16_t *distance_cm){
	if(start_flag != true){
		return DISTANCE_NOT_READY;
	}
	if(sr04_status != SR04_OK){
		return DISTANCE_IN_PROGRESS;
	}
	start_flag = false;
	*distance_cm = (echo_us * 17)  / 1000;
	return DISTANCE_OK;
}

void distance_ISR(void){
	sr04_echoISR();
}

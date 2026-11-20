/*
 * ap_main.c
 *
 *  Created on: 2026. 10. 28.
 *      Author: kccistc
 */

// include
#include "ap_main.h"
#include "service/temp_humid/temp_humid.h"
#include "service/distance/distance.h"
#include "service/display/display.h"
#include "../driver/button/button.h"
#include <stdbool.h>
#include <stdio.h>

// define
#define DISTANCE_MIN_INTERVAL_MS	60

#define C5_PERIOD	1911
#define D5_PERIOD	1703
#define E5_PERIOD	1517
#define F5_PERIOD	1432
#define G5_PERIOD	1276
#define A5_PERIOD	1136
#define B5_PERIOD	1012
#define C6_PERIOD	956

#define	ONE		2000
#define	TWO		1000
#define	FOUR	500
#define	EIGHT	250

// typedef

// variable declarations

// variable definitions
extern TIM_HandleTypeDef htim1;

// function declarations
int __io_putchar(int ch);

// function definitions
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == GPIO_PIN_14){
		distance_ISR();
	}
}

void c5(void){
	__HAL_TIM_SET_AUTORELOAD(&htim1, C5_PERIOD-1);
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, C5_PERIOD/2);
}

void d5(void){
	__HAL_TIM_SET_AUTORELOAD(&htim1, D5_PERIOD-1);
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, D5_PERIOD/2);
}

void e5(void){
	__HAL_TIM_SET_AUTORELOAD(&htim1, E5_PERIOD-1);
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, E5_PERIOD/2);
}

void f5(void){
	__HAL_TIM_SET_AUTORELOAD(&htim1, F5_PERIOD-1);
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, F5_PERIOD/2);
}

void g5(void){
	__HAL_TIM_SET_AUTORELOAD(&htim1, G5_PERIOD-1);
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, G5_PERIOD/2);
}
void a5(void){
	__HAL_TIM_SET_AUTORELOAD(&htim1, A5_PERIOD-1);
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, A5_PERIOD/2);
}

void b5(void){
	__HAL_TIM_SET_AUTORELOAD(&htim1, B5_PERIOD-1);
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, B5_PERIOD/2);
}

void c6(void){
	__HAL_TIM_SET_AUTORELOAD(&htim1, C6_PERIOD-1);
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, C6_PERIOD/2);
}

int ap_main(void){
	uint16_t dist_cm;
	button_t btn_distance;
	TH_init();
	distance_init();
	display_init();
	button_init(&btn_distance, GPIOC, GPIO_PIN_0);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
	while(1){
		c5();
		HAL_Delay(EIGHT);
		d5();
		HAL_Delay(EIGHT);
		e5();
		HAL_Delay(EIGHT);
		f5();
		HAL_Delay(EIGHT);
		g5();
		HAL_Delay(EIGHT);
		a5();
		HAL_Delay(EIGHT);
		b5();
		HAL_Delay(EIGHT);
		c6();
		HAL_Delay(EIGHT);
	}
	while(1){
		if(button_getState(&btn_distance) == ACT_PUSH){
			distance_request();
			printf("distance requested!\n");
		}
		if(distance_get(&dist_cm) == DISTANCE_OK){
			printf("distance %3u cm\n\n", dist_cm);
		}
		TH_task();
		distance_task();
		display_THtask();
		display_distanceTask();
	}
	return 0;
}

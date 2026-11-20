/*
 * joystick.c
 *
 *  Created on: 2026. 11. 3.
 *      Author: kccistc
 */

// include
#include "joystick.h"

// define
#define ADC_CH_COUNT	2
#define ADC_RESOLUTION	1024

// typedef

// variable declarations
extern UART_HandleTypeDef huart2;

// variable definitions
static ADC_HandleTypeDef *joy_adc;
static uint8_t joystick_ready;
static volatile uint8_t adcCpltFlag = 0;
static uint16_t value[ADC_CH_COUNT] = {0};	// value[0] : x-axis, value[1] : y-axis
static uint16_t pout[2];
static uint16_t vout[2];

// function declarations
int __io_putchar(int ch){
	HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
	return ch;
}

// function definitions
void joystick_init(ADC_HandleTypeDef *hadc)
{
	joy_adc = hadc;
	joystick_ready = 0;
	if(joy_adc == 0 || joy_adc ->DMA_Handle == 0)
	{
		return;
	}
	if (HAL_ADC_Start_DMA(joy_adc, (uint32_t *)(void *)value, 2U) != HAL_OK)
			{
				return;
			}
	joystick_ready = 1;
}

HAL_StatusTypeDef joystick_read(joystick_data_t *data){
	if(adcCpltFlag == 1){
	data -> x = value[0];
	data -> y = value[1];
	}
	return HAL_OK;
}

void joystic_print(void){
	if(adcCpltFlag == 1){
		adcCpltFlag = 0;
		pout[0] = (uint16_t)((ADC_RESOLUTION - value[0]) * 100  / ADC_RESOLUTION);
		vout[0] = (uint16_t)(value[0] * 3300  / ADC_RESOLUTION);
		pout[1] = (uint16_t)((ADC_RESOLUTION - value[1]) * 100  / ADC_RESOLUTION);
		vout[1] = (uint16_t)(value[1] * 3300  / ADC_RESOLUTION);
		printf("v1 : %d, p1 : %d%%, voltage1 : %d\nv2 : %d, p2 : %d%%, voltage2 : %d\n\n", value[0], pout[0], vout[0], value[1], pout[1], vout[1]);
	}
}

void joystick_ISR(void){
	adcCpltFlag = 1;
}

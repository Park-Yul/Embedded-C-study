/*
 * fnd.c
 *
 *  Created on: 2026. 10. 14.
 *      Author: kccistc
 */

#include "fnd.h"

#define FND_DIGIT_1		1
#define FND_DIGIT_2		2
#define FND_DIGIT_3		3
#define FND_DIGIT_4		4

typedef struct {
	GPIO_TypeDef* GPIOx;
	uint16_t GPIO_Pin;
} set_port_t;

static void fnd_all_off(void);
static void fnd_sel_digit(int digit);
static void fnd_disp_number(int number);

static void fnd_all_off(void)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
}

static void fnd_sel_digit(int digit)
{
	switch(digit)
	{
	case 1:
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
		break;
	case 2:
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
		break;
	case 3:
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
		break;
	case 4 :
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);
		break;

	}
}

static void fnd_disp_number(int number)
{
	set_port_t seg_font[8] = {
			{GPIOA, GPIO_PIN_6},	// A
			{GPIOA, GPIO_PIN_9},	// B
			{GPIOB, GPIO_PIN_5},	// C
			{GPIOB, GPIO_PIN_10},	// D
			{GPIOA, GPIO_PIN_8},	// E
			{GPIOA, GPIO_PIN_7},	// F
			{GPIOB, GPIO_PIN_3},	// G
			{GPIOB, GPIO_PIN_4},	// DP
	};
	int seg_font_data[10][8] = {
			{1, 1, 1, 1, 1, 1, 0, 0}, // 0
			{0, 1, 1, 0, 0, 0, 0, 0}, // 1
			{1, 1, 0, 1, 1, 0, 1, 0}, // 2
			{1, 1, 1, 1, 0, 0, 1, 0}, // 3
			{0, 1, 1, 0, 0, 1, 1, 0}, // 4
			{1, 0, 1, 1, 0, 1, 1, 0}, // 5
			{1, 0, 1, 1, 1, 1, 1, 0}, // 6
			{1, 1, 1, 0, 0, 1, 0, 0}, // 7
			{1, 1, 1, 1, 1, 1, 1, 0}, // 8
			{1, 1, 1, 1, 0, 1, 1, 0}  // 9
	};

	for(int i=0; i<8 ;i++) {
		HAL_GPIO_WritePin(seg_font[i].GPIOx, seg_font[i].GPIO_Pin, seg_font_data[number][i]);
	}
}

void fnd_show_number(int number)
{
	static int state = 0;	// static variable

	int d1, d2, d3, d4;
	d1 = (number / 1000) % 10;
	d2 = (number / 100) % 10;
	d3 = (number / 10) % 10;
	d4 = (number / 1) % 10;

	fnd_all_off();
	switch(state)
	{
	case 0 :
		fnd_sel_digit(FND_DIGIT_1);
		fnd_disp_number(d1);
		HAL_Delay(0);
		break;

	case 1:
		fnd_sel_digit(FND_DIGIT_2);
		fnd_disp_number(d2);
		HAL_Delay(0);
		break;

	case 2:
		fnd_sel_digit(FND_DIGIT_3);
		fnd_disp_number(d3);
		HAL_Delay(0);
		break;

	case  3:
		fnd_sel_digit(FND_DIGIT_4);
		fnd_disp_number(d4);
		HAL_Delay(0);
		break;

	default:
		break;
	}
	state = (state + 1) % 4;
}

//fnd_show_number(counter);
//
//if(HAL_GetTick() - prevtime >= 100) {
//	prevtime = HAL_GetTick();
//
//	if(counter >= 9999){
//		counter = 0;
//	}
//	else{
//		counter ++;
//	}
//}

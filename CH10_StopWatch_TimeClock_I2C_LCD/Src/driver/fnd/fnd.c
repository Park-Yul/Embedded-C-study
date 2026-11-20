/*
 * fnd.c
 *
 *  Created on: 2026. 10. 14.
 *      Author: kccistc
 */

#include "fnd.h"


// variables
static uint16_t num = 0;
static uint16_t dp = 0;

// function prototypes
void fnd_set_dp(int dig, uint16_t state);
uint16_t fnd_get_dp(void);
void fnd_set_num(uint16_t n);
uint16_t fnd_get_num(void);
void fnd_show_number(void);
static void fnd_all_off(void);
static void fnd_sel_digit(int digit);
static void fnd_disp_number(int number);

// function definitions
void fnd_set_dp(int dig, uint16_t state)
{
	if(state == FND_DP_ON){
		dp |= (1<<dig);	// ex) dp |= (1<<FND_DIGIT); dp |= (1<<1); => 2bxx1x
	}
	else{
		dp &= ~(1<<dig); // ex) dp &= (1<<FND_DIGIT); dp &= ~(1<<1); => 2bxx0x
	}
}

uint16_t fnd_get_dp(void)
{
	return dp;
}

void fnd_set_num(uint16_t n)
{
	num = n;
}

uint16_t fnd_get_num(void)
{
	return num;
}

void fnd_show_number(void)
{
	static int state = 0;	// static variable

	int number = fnd_get_num();
	int dpData = fnd_get_dp();

	int d1, d2, d3, d4;
	d1 = (number / 1000) % 10;
	d2 = (number / 100) % 10;
	d3 = (number / 10) % 10;
	d4 = (number / 1) % 10;

	fnd_all_off();
	switch(state)
	{
	case 0 :	// digit 1, 1000
		fnd_sel_digit(FND_DIGIT_1);
		fnd_disp_number(d1);
		break;
	case 1:		// digit 2, 100
		fnd_sel_digit(FND_DIGIT_2);
		fnd_disp_number(d2);
		break;
	case 2:		// digit 3, 10
		fnd_sel_digit(FND_DIGIT_3);
		fnd_disp_number(d3);
		break;
	case 3:		// digit 4, 1
		fnd_sel_digit(FND_DIGIT_4);
		fnd_disp_number(d4);
		break;
	case 4 :	// digit 1, dp
		if((dpData & (1<<FND_DIGIT_1)) ==0){
			fnd_disp_number(SEG_DP_OFF);	// dp off
		}
		else {
			fnd_disp_number(SEG_DP_ON);	// dp on
		}
		fnd_sel_digit(FND_DIGIT_1);
		break;
	case 5:		// digit 2, dp
		if((dpData & (1<<FND_DIGIT_2)) ==0){
			fnd_disp_number(SEG_DP_OFF);	// dp off
		}
		else {
			fnd_disp_number(SEG_DP_ON);	// dp on
		}
		fnd_sel_digit(FND_DIGIT_2);
		break;
	case 6:		// digit 3, dp
		if((dpData & (1<<FND_DIGIT_3)) ==0){
			fnd_disp_number(SEG_DP_OFF);	// dp off
		}
		else {
			fnd_disp_number(SEG_DP_ON);	// dp on
		}
		fnd_sel_digit(FND_DIGIT_3);
		break;
	case 7:		// digit 4, dp
		if((dpData & (1<<FND_DIGIT_4)) ==0){
			fnd_disp_number(SEG_DP_OFF);	// dp off
		}
		else {
			fnd_disp_number(SEG_DP_ON);	// dp on
		}
		fnd_sel_digit(FND_DIGIT_4);
		break;
	default:
		break;
	}
	state = (state + 1) % 8;
}

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
	int seg_font_data[12][8] = {
			{1, 1, 1, 1, 1, 1, 0, 0}, // 0
			{0, 1, 1, 0, 0, 0, 0, 0}, // 1
			{1, 1, 0, 1, 1, 0, 1, 0}, // 2
			{1, 1, 1, 1, 0, 0, 1, 0}, // 3
			{0, 1, 1, 0, 0, 1, 1, 0}, // 4
			{1, 0, 1, 1, 0, 1, 1, 0}, // 5
			{1, 0, 1, 1, 1, 1, 1, 0}, // 6
			{1, 1, 1, 0, 0, 1, 0, 0}, // 7
			{1, 1, 1, 1, 1, 1, 1, 0}, // 8
			{1, 1, 1, 1, 0, 1, 1, 0}, // 9
			{0, 0, 0, 0, 0, 0, 0, 1}, // 10, dp on
			{0, 0, 0, 0, 0, 0, 0, 0}  // 11, dp off
	};

	for(int i=0; i<8 ;i++) {
		HAL_GPIO_WritePin(seg_font[i].GPIOx, seg_font[i].GPIO_Pin, seg_font_data[number][i]);
	}
}

/*
 * display.c
 *
 *  Created on: 2026. 11. 3.
 *      Author: kccistc
 */

// include
#include "display.h"
#include "../../../driver/lcd/lcd.h"

// define

// typedef

// variable declarations
extern I2C_HandleTypeDef hi2c1;

// variable definitions

// function declarations
static void disp_Xcount(int32_t data);
static void disp_Ycount(int32_t data);

// function definitions
void disp_init(void){
	Lcd_init(&hi2c1);
	char str1[30];
	sprintf(str1, "Welcome to           ");
	Lcd_writeStringXY(0, 0, str1);
	char str2[30];
	sprintf(str2, "Counting STM32!!         ");
	Lcd_writeStringXY(0, 1, str2);
	HAL_Delay(1000);
}

void disp_task(int32_t x, int32_t y){
	disp_Xcount(x);
	disp_Ycount(y);
	HAL_Delay(5);
}

static void disp_Xcount(int32_t data){
	char str[30];
	sprintf(str, "X=%d               ", data);
	Lcd_writeStringXY(0, 0, str);
}

static void disp_Ycount(int32_t data){
	char str[30];
	sprintf(str, "Y=%d               ", data);
	Lcd_writeStringXY(0, 1, str);
}

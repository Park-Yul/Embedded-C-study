/*
 * lcd.h
 *
 *  Created on: 2026. 10. 21.
 *      Author: kccistc
 */

#ifndef SRC_DRIVER_LCD_LCD_H_
#define SRC_DRIVER_LCD_LCD_H_

// include
#include "stm32f4xx_hal.h"
#include <stdio.h>

// define

// typedef

// variable declarations

// function declarations
void Lcd_init(I2C_HandleTypeDef *lcd_hi2c);
void Lcd_writeStringXY(uint8_t col, uint8_t row, char *str);

#endif /* SRC_DRIVER_LCD_LCD_H_ */

/*
 * button_task.c
 *
 *  Created on: 2026. 11. 12.
 *      Author: kccistc
 */


/* include */
#include "button_task.h"
#include "../../message.h"
#include "../../../driver/button/button.h"
#include "../../../driver/lcd/lcd.h"

/* define */

/* typedef */

/* variable declarations */

/* variable definitions */
static button_t btn_run_stop, btn_right, btn_left;

/* function declarations */

/* function definitions */
void button_task_init(void){
	button_init(&btn_run_stop, GPIOC, GPIO_PIN_1);
	button_init(&btn_right, GPIOC, GPIO_PIN_2);
	button_init(&btn_left, GPIOC, GPIO_PIN_0);
}

void button_task(void){
	if(button_getState(&btn_run_stop) == ACT_PUSH){
		osMessagePut(btnQueHandle, BTN_RUN_STOP, 10);
		if(osMutexWait(lcdMutexHandle, osWaitForever) == osOK){
			Lcd_writeStringXY(0, 1, "BTN RUNSTOP    ");
			osMutexRelease(lcdMutexHandle);
		}
	}
	else if(button_getState(&btn_right) == ACT_PUSH){
		osMessagePut(btnQueHandle, BTN_RIGHT, 10);
		if(osMutexWait(lcdMutexHandle, osWaitForever) == osOK){
			Lcd_writeStringXY(0, 1,  "BTN RIGHT       ");
			osMutexRelease(lcdMutexHandle);
		}
	}
	else if(button_getState(&btn_left) == ACT_PUSH){
		osMessagePut(btnQueHandle, BTN_LEFT, 10);
		if(osMutexWait(lcdMutexHandle, osWaitForever) == osOK){
			Lcd_writeStringXY(0, 1, "BTN LEFT        ");
			osMutexRelease(lcdMutexHandle);
		}
	}

}

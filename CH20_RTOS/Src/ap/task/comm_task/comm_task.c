/*
 * comm_task.c
 *
 *  Created on: 2026. 11. 12.
 *      Author: kccistc
 */

/* include */
#include "comm_task.h"
#include "../../message.h"
#include "../../../driver/lcd/lcd.h"
#include "usart.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* define */

/* typedef */

/* variable declarations */

/* variable definitions */
static uint8_t rx_data;
static uint8_t rx_index;
static char rx_buff[32];
static bool rx_cplt_flag = false;
static comm_data_t comm_data;

/* function declarations */

/* function definitions */
void comm_task_init(void){
	HAL_UART_Receive_IT(&huart2, &rx_data, 1);
}

void comm_task(void){
	if(rx_cplt_flag){
		rx_cplt_flag = false;
		if(strncmp(rx_buff, "RUN", 3) == 0){
			comm_data.status = COMM_RUN;
			osMessagePut(commQueHandle, (uint32_t)&comm_data, 10);
			if(osMutexWait(lcdMutexHandle, osWaitForever) == osOK){
				Lcd_writeStringXY(0, 1, "Comm Task RUN   ");
				osMutexRelease(lcdMutexHandle);
			}
		}

		else if(strncmp(rx_buff, "STOP", 4) == 0){
			comm_data.status = COMM_STOP;
			osMessagePut(commQueHandle, (uint32_t)&comm_data, 10);
			if(osMutexWait(lcdMutexHandle, osWaitForever) == osOK){
				Lcd_writeStringXY(0, 1, "Comm Task STOP  ");
				osMutexRelease(lcdMutexHandle);
			}
		}
		else if(strncmp(rx_buff, "SPD", 3) == 0){
			comm_data.status = COMM_SPEED;
			comm_data.speed = atoi(&rx_buff[3]);
			osMessagePut(commQueHandle, (uint32_t)&comm_data, 10);
			if(osMutexWait(lcdMutexHandle, osWaitForever) == osOK){
				char buf[30];
				sprintf(buf, "Comm SPD %03d    ", comm_data.speed);
				Lcd_writeStringXY(0, 1, buf);
				osMutexRelease(lcdMutexHandle);
			}
		}
		else{
			osMessagePut(commQueHandle, COMM_NO, 10);
			if(osMutexWait(lcdMutexHandle, osWaitForever) == osOK){
				Lcd_writeStringXY(0, 1, "Comm Task NO    ");
				osMutexRelease(lcdMutexHandle);
			}
		}
		rx_index = 0;
	}

}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if(huart->Instance == USART2){
		if(rx_data == '*'){
			rx_buff[rx_index++] = '\0';
			rx_cplt_flag = true;
		}
		else {
			rx_buff[rx_index++] =  rx_data;
		}
		HAL_UART_Receive_IT(&huart2, &rx_data, 1);
	}
}

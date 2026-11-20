/*
 * motor_control_task.c
 *
 *  Created on: 2026. 11. 12.
 *      Author: kccistc
 */

/* include */
#include "motor_control_task.h"
#include "../sound_task/sound_task.h"
#include "../../message.h"
#include "../../../driver/button/button.h"
#include "../../../driver/dc_motor/dc_motor.h"
#include "../../../driver/lcd/lcd.h"

/* define */

/* typedef */


/* variable declarations */
extern TIM_HandleTypeDef htim3;

/* variable definitions */
static motor_t motor_r, motor_l;
motor_state_t motor_state = STOP;
static btn_status_t btn_status = BTN_NO;
//static comm_status_t comm_status =  COMM_NO;
static comm_data_t pcomm_data_dummy = {0};
static comm_data_t *pcomm_data;
/* function declarations */

/* function definitions */
void motor_control_task_init(void){
	motor_init(&motor_r, &htim3, TIM_CHANNEL_1, GPIOB, GPIO_PIN_6, GPIOA, GPIO_PIN_7);
	motor_init(&motor_l, &htim3, TIM_CHANNEL_3, GPIOA, GPIO_PIN_6, GPIOA, GPIO_PIN_5);
	motor_set_speed(500);
}

void motor_control_task(void){
	osEvent btn_evt;
	btn_evt = osMessageGet(btnQueHandle, 0);
	if(btn_evt.status == osEventMessage){
		btn_status = btn_evt.value.v;
	}
	else{
		btn_status = BTN_NO;
	}
	osEvent comm_evt;
	comm_evt = osMessageGet(commQueHandle, 0);
	if(comm_evt.status == osEventMessage){
		pcomm_data = comm_evt.value.p;
	}
	else{
		//		comm_status = COMM_NO;
	}

	if(pcomm_data->status == COMM_SPEED){
		motor_set_speed(pcomm_data->speed);
	}

	switch(motor_state){
	case STOP:
		if((btn_status == BTN_RUN_STOP) || (pcomm_data->status == COMM_RUN)){
			motor_state = RUN;
		}
		else if(btn_status == BTN_RIGHT){
			motor_state = RIGHT;
		}
		else if(btn_status == BTN_LEFT){
			motor_state = LEFT;
		}
		motor_stop(&motor_r, &motor_l);
		if(osMutexWait(lcdMutexHandle, osWaitForever) == osOK){
			Lcd_writeStringXY(0, 0, "Motor Task STOP ");
			osMutexRelease(lcdMutexHandle);
		}
		break;
	case RUN:
		if((btn_status == BTN_RUN_STOP) || (pcomm_data->status == COMM_STOP)){
			sound_flag = true;
			motor_state = STOP;
		}
		else if(btn_status == BTN_RIGHT){
			motor_state = RIGHT;
		}
		else if(btn_status == BTN_LEFT){
			motor_state = LEFT;
		}
		motor_forward(&motor_r, &motor_l);
		if(osMutexWait(lcdMutexHandle, osWaitForever) == osOK){
			Lcd_writeStringXY(0, 0, "Motor Task RUN  ");
			osMutexRelease(lcdMutexHandle);
		}
		break;
	case RIGHT:
		if((btn_status == BTN_RUN_STOP) || (pcomm_data->status == COMM_STOP)){
			sound_flag = true;
			motor_state = STOP;
		}
		else if(btn_status == BTN_RIGHT){
			motor_state = RIGHT;
		}
		else if(btn_status == BTN_LEFT){
			motor_state = LEFT;
		}
		motor_right(&motor_r, &motor_l);
		if(osMutexWait(lcdMutexHandle, osWaitForever) == osOK){
			Lcd_writeStringXY(0, 0, "Motor Task RIGHT");
			osMutexRelease(lcdMutexHandle);
		}
		break;
	case LEFT:
		if((btn_status == BTN_RUN_STOP) || (pcomm_data->status == COMM_STOP)){
			sound_flag = true;
			motor_state = STOP;
		}
		else if(btn_status == BTN_RIGHT){
			motor_state = RIGHT;
		}
		else if(btn_status == BTN_LEFT){
			motor_state = LEFT;
		}
		motor_left(&motor_r, &motor_l);
		if(osMutexWait(lcdMutexHandle, osWaitForever) == osOK){
			Lcd_writeStringXY(0, 0, "Motor Task LEFT ");
			osMutexRelease(lcdMutexHandle);
		}
		break;
	default:
		sound_flag = true;
		motor_state = STOP;
		break;
	}
}

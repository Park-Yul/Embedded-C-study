/*
 * message.h
 *
 *  Created on: 2026. 11. 12.
 *      Author: kccistc
 */

#ifndef SRC_AP_MESSAGE_H_
#define SRC_AP_MESSAGE_H_

/* include */
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"

/* define */

/* typedef */
typedef enum{
	BTN_NO,
	BTN_RUN_STOP,
	BTN_RIGHT,
	BTN_LEFT
}btn_status_t;

typedef enum{
	COMM_NO,
	COMM_STOP,
	COMM_RUN,
	COMM_SPEED
}comm_status_t;

typedef struct{
	comm_status_t status;
	int speed;
}comm_data_t;

/* variable declarations */
extern osMessageQId btnQueHandle;
extern osMessageQId commQueHandle;
extern osMutexId lcdMutexHandle;

/* function declarations */

#endif /* SRC_AP_MESSAGE_H_ */



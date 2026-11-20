/*
 * ap_main.c
 *
 *  Created on: 2026. 10. 30.
 *      Author: kccistc
 */

// include
#include "ap_main.h"
#include "service/sound/sound.h"
#include "../common/queue/que_buffer.h"
#include "../driver/button/button.h"
#include "../driver/lcd/lcd.h"
#include <stdio.h>

// define

// typedef

// variable declarations
extern I2C_HandleTypeDef hi2c1;

// variable definitions
button_t btn_normal, btn_error, btn_alarm;

que_buf_t que_btnEvent;
btn_event_t btnEvent[BTN_EVENT_Q_SIZE];



// function declarations
static void ap_init(void);

// function definitions
int ap_main(void){
	uint32_t prevTime = 0;
	btn_event_t btnEvent;
	ap_init();
	while(1){
		if(button_getState(&btn_normal) == ACT_RELEASE){
			btnEvent = BTN_NORMAL;
			que_put(&que_btnEvent, &btnEvent);
		}
		if(button_getState(&btn_error) == ACT_RELEASE){
			btnEvent = BTN_ERROR;
			que_put(&que_btnEvent, &btnEvent);
		}
		if(button_getState(&btn_alarm) == ACT_RELEASE){
			btnEvent = BTN_ALARM;
			que_put(&que_btnEvent, &btnEvent);
		}

		if(HAL_GetTick() - prevTime >= 1000) {
			prevTime = HAL_GetTick();

			if(!que_is_empty(&que_btnEvent)){
				que_get(&que_btnEvent, &btnEvent);

				if(btnEvent == BTN_NORMAL){
					sound_button();
				}
				if(btnEvent == BTN_ERROR){
					sound_error();
				}
				if(btnEvent == BTN_ALARM){
					sound_alarm();
				}
			}
		}
	}
}

static void ap_init(void){
	button_init(&btn_normal, GPIOC, GPIO_PIN_0);
	button_init(&btn_error, GPIOC, GPIO_PIN_1);
	button_init(&btn_alarm, GPIOC, GPIO_PIN_3);
	Lcd_init(&hi2c1);
	char buff[30];
	sprintf(buff, "Hello!");
	Lcd_writeStringXY(0, 0, buff);
	que_buf_init(&que_btnEvent, btnEvent, sizeof(btn_event_t), BTN_EVENT_Q_SIZE);
	sound_init();
	sound_power();
}

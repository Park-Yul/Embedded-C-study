/*
 * ap_main.c
 *
 *  Created on: 2026. 10. 16.
 *      Author: kccistc
 */
#include "ap_main.h"
#include "TimeClock/TimeClock.h"
#include "StopWatch/StopWatch.h"
#include "../driver/led/led.h"
#include "../driver/button/button.h"
#include "../driver/fnd/fnd.h"

button_t btn_Mode;
led_t led_TimeClock, led_StopWatch;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM3) {	// every 1ms
		fnd_show_number();
	}
	else if(htim->Instance ==TIM4){		// every 10ms
		TimeClock_ISR();
		StopWatch_ISR();
	}
}

int ap_main(void)
{
	TimeClock_init();
	StopWatch_init();
	button_init(&btn_Mode, GPIOC, GPIO_PIN_3);
	led_init(&led_TimeClock, GPIOB, GPIO_PIN_15);
	led_init(&led_StopWatch, GPIOB, GPIO_PIN_14);

	modeState_t modeState = TIME_CLOCK_MODE;
	while(1) {
		switch(modeState){
		case TIME_CLOCK_MODE:
			if(button_getState(&btn_Mode) == ACT_PUSH){
				modeState = STOP_WATCH_MODE;
			}
			TimeClock_task();
			led_on(&led_TimeClock);
			led_off(&led_StopWatch);
			break;
		case STOP_WATCH_MODE:
			if(button_getState(&btn_Mode) == ACT_PUSH){
				modeState = TIME_CLOCK_MODE;
			}
			StopWatch_task();
			led_off(&led_TimeClock);
			led_on(&led_StopWatch);
			break;
		default:
			break;
		}
	}
	return 0;
}

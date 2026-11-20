/*
 * ap_main.c
 *
 *  Created on: 2026. 10. 16.
 *      Author: kccistc
 */
#include "ap_main.h"
#include "TimeClock/TimeClock.h"
#include "StopWatch/StopWatch.h"
#include "Display/Display.h"
//#include "../driver/led/led.h"
#include "../driver/button/button.h"
#include "../driver/fnd/fnd.h"

// variables
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
static button_t btn_Mode;
static modeState_t modeState = TIME_CLOCK_MODE;

// function prototypes
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
int ap_main(void);
static void ap_init(void);
static void ap_TimeClockMode(void);
static void ap_StopWatchMode(void);
static void ap_LcdDisplay(void);

// function definitions
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
	ap_init();
	while(1) {
		switch(modeState){
		case TIME_CLOCK_MODE:
			ap_TimeClockMode();
			break;
		case STOP_WATCH_MODE:
			ap_StopWatchMode();
			break;
		default:
			break;
		}
		ap_LcdDisplay();
	}
	return 0;
}

static void ap_init(void){
	TimeClock_init();
	StopWatch_init();
	Disp_init();
	Disp_setMode(modeState);
	StopWatch_state_t StopWatchState = StopWatch_getState();
	Disp_setStopWatchState(StopWatchState);

	TimeClock_t *pTimeClockTime = TimeClock_getTimeData();
	Disp_lcdTimeClockTime((TimeClockTime_t *)pTimeClockTime);

	StopWatch_t *pStopWatchTime = StopWatch_getTimeData();
	Disp_lcdStopWatchTime((StopWatchTime_t *)pStopWatchTime);

	button_init(&btn_Mode, GPIOC, GPIO_PIN_3);
	HAL_TIM_Base_Start_IT(&htim3);
	HAL_TIM_Base_Start_IT(&htim4);
}

static void ap_TimeClockMode(void){
	if(button_getState(&btn_Mode) == ACT_PUSH){
		modeState = STOP_WATCH_MODE;
	}
	TimeClock_t *pTimeClockTime = TimeClock_getTimeData();
	Disp_setTimeClockTime((TimeClockTime_t *) pTimeClockTime);

	Disp_setMode(modeState);
}

static void ap_StopWatchMode(void){
	if(button_getState(&btn_Mode) == ACT_PUSH){
		modeState = TIME_CLOCK_MODE;
	}
	StopWatch_task();

	StopWatch_t *pStopWatchTime = StopWatch_getTimeData();
	Disp_setStopWatchTime((StopWatchTime_t *) pStopWatchTime);

	Disp_setMode(modeState);
	StopWatch_state_t StopWatchState = StopWatch_getState();
	Disp_setStopWatchState(StopWatchState);
}

static void ap_LcdDisplay(void){
	TimeClock_t *pTimeClockTime = TimeClock_getTimeData();
	Disp_lcdTimeClockTime((TimeClockTime_t *)pTimeClockTime);

	StopWatch_t *pStopWatchTime = StopWatch_getTimeData();
	Disp_lcdStopWatchTime((StopWatchTime_t *)pStopWatchTime);
}

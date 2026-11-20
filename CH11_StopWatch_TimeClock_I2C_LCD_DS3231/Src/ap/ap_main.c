/*
 * ap_main.c
 *
 *  Created on: 2026. 10. 16.
 *      Author: kccistc
 */

//include
#include <stdio.h>
#include "ap_main.h"
#include "TimeClock/TimeClock.h"
#include "StopWatch/StopWatch.h"
#include "Display/Display.h"
#include "../driver/button/button.h"
#include "../driver/fnd/fnd.h"

// variables
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim5;
extern UART_HandleTypeDef huart2;
static button_t btn_Mode;
static modeState_t modeState = TIME_CLOCK_MODE;

// function prototypes
static void ap_init(void);
static void ap_TimeClockMode(void);
static void ap_StopWatchMode(void);
void uart_ISR(void);
//int __io_putchar(int ch);

// function definitions
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM3) {	// every 1ms
		fnd_show_number();
	}
	else if (htim->Instance == TIM4) {		// every 10ms
		TimeClock_ISR();
		StopWatch_ISR();
	}
	else if (htim->Instance == TIM5) {		// every 1s
		uart_ISR();
	}
}

int ap_main(void) {
	ap_init();
	while (1) {
		switch (modeState) {
		case TIME_CLOCK_MODE:
			ap_TimeClockMode();
			break;
		case STOP_WATCH_MODE:
			ap_StopWatchMode();
			break;
		default:
			break;
		}
	}
	return 0;
}

static void ap_init(void) {
	TimeClock_init();
	StopWatch_init();
	Disp_init();
	Disp_setMode(modeState);
	StopWatch_state_t StopWatchState = StopWatch_getState();
	Disp_setStopWatchState(StopWatchState);
	TimeClock_t *pTimeClockTime = TimeClock_getTimeData();
	Disp_setTimeClockTime((TimeClockTime_t*) pTimeClockTime);
	TimeDate_t *pTimeClockDate = TimeClock_getDateData();
	Disp_setTimeClockDate((TimeClockDate_t*) pTimeClockDate);
	StopWatch_t *pStopWatchTime = StopWatch_getTimeData();
	Disp_setStopWatchTime((StopWatchTime_t*) pStopWatchTime);
	button_init(&btn_Mode, GPIOC, GPIO_PIN_3);
	HAL_TIM_Base_Start_IT(&htim3);
	HAL_TIM_Base_Start_IT(&htim4);
	HAL_TIM_Base_Start_IT(&htim5);
}

static void ap_TimeClockMode(void) {
	if (button_getState(&btn_Mode) == ACT_PUSH) {
		modeState = STOP_WATCH_MODE;
	}
	TimeClock_task();
	TimeClock_t *pTimeClockTime = TimeClock_getTimeData();
	Disp_setTimeClockTime((TimeClockTime_t*) pTimeClockTime);
	TimeDate_t *pTimeClockDate = TimeClock_getDateData();
	Disp_setTimeClockDate((TimeClockDate_t*) pTimeClockDate);
	Disp_setMode(modeState);
}

static void ap_StopWatchMode(void) {
	if (button_getState(&btn_Mode) == ACT_PUSH) {
		modeState = TIME_CLOCK_MODE;
	}
	StopWatch_task();
	StopWatch_t *pStopWatchTime = StopWatch_getTimeData();
	Disp_setStopWatchTime((StopWatchTime_t*) pStopWatchTime);
	StopWatch_state_t StopWatchState = StopWatch_getState();
	Disp_setStopWatchState(StopWatchState);
	Disp_setMode(modeState);
}

// int __io_putchar(int ch){
// 	HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 10);
// 	return ch;
// }

void uart_ISR(void)
{
    char buffer[80];

    TimeDate_t *date = TimeClock_getDateData();
    TimeClock_t *time = TimeClock_getTimeData();

    const char *day[7] = {
        "SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"
    };

    int len = snprintf(
        buffer,
        sizeof(buffer),
        "20%02d-%02d-%02d %s\r\n"
        "%02d:%02d:%02d\r\n\r\n",
        date->year,
        date->month,
        date->date,
        day[date->day - 1],
        time->hour,
        time->min,
        time->sec
    );

    if (len > 0) {
        HAL_UART_Transmit(
            &huart2,
            (uint8_t *)buffer,
            (uint16_t)len,
            10
        );
    }
}

/*
 * smart_watch.c
 *
 *  Created on: 2026. 10. 31.
 *      Author: kccistc
 */

// include
#include "smart_watch.h"
#include "../ap/service/TimeClock/TimeClock.h"
#include "../ap/service/StopWatch/StopWatch.h"
#include "../ap/service/temp_humid/temp_humid.h"
#include "../ap/service/Display/Display.h"
#include "../ap/service/distance/distance.h"
#include "../ap/service/sound/sound.h"
#include "../driver/button/button.h"
#include "../driver/fnd/fnd.h"
#include "../common/delay/delay.h"
#include "../common/queue/que_buffer.h"

// define

// typedef
typedef enum {
	TIME_CLOCK_MODE,
	TEMP_HUMID_MODE,
	DISTANCE_MODE,
	STOP_WATCH_MODE
} modeState_t;

// variable declarations
extern TIM_HandleTypeDef htim3;		// for fnd interrupt
extern TIM_HandleTypeDef htim4;		// for TimeClock interrupt
extern TIM_HandleTypeDef htim5;		// for UART interrupt
extern UART_HandleTypeDef huart2; 	// for UART

// variable definitions
static que_buf_t que_btnEvent;
static btn_event_t btnEvent[BTN_EVENT_Q_SIZE];
static modeState_t modeState = TIME_CLOCK_MODE;
static button_t btn_mode, btn_runstop, btn_clear;
static uint16_t dist_cm;

// function declarations
static void smart_watch_init(void);
static void smart_watch_btn(void);
static void smart_watch_task(void);
static void smart_watch_TimeClockMode(void);
static void smart_watch_TempHumidMode(void);
static void smart_watch_DistanceMode(void);
static void smart_watch_StopWatchMode(void);
static void uart_ISR(void);

// function definitions
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM3) {				// every 1ms
		fnd_show_number();
	}
	else if (htim->Instance == TIM4) {			// every 10ms
		TimeClock_ISR();
		StopWatch_ISR();
	}
	else if (htim->Instance == TIM5) {			// every 1s
		uart_ISR();
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == GPIO_PIN_14){
		distance_ISR();
	}
}

int smart_watch_main(void){
	smart_watch_init();
	while(1){
		smart_watch_btn();
		smart_watch_task();
	}
}

static void smart_watch_init(void){
    HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
    HAL_NVIC_SetPriority(TIM3_IRQn, 1, 0);
    HAL_NVIC_SetPriority(TIM4_IRQn, 1, 0);
    HAL_NVIC_SetPriority(TIM5_IRQn, 1, 0);
	button_init(&btn_mode, GPIOC, GPIO_PIN_3);
	button_init(&btn_runstop, GPIOC, GPIO_PIN_0);
	button_init(&btn_clear, GPIOC, GPIO_PIN_1);
	TimeClock_init();
	StopWatch_init();
	Disp_init();
	TH_init();
	distance_init();
	sound_init();
	que_buf_init(&que_btnEvent, btnEvent, sizeof(btn_event_t), BTN_EVENT_Q_SIZE);
	HAL_TIM_Base_Start_IT(&htim3);
	HAL_TIM_Base_Start_IT(&htim4);
	HAL_TIM_Base_Start_IT(&htim5);
}

static void smart_watch_btn(void){
	static uint32_t prevTime = 0;
	static btn_event_t btnEvent;
	if(button_getState(&btn_mode) == ACT_PUSH){
		btnEvent = BTN_MODE;
		que_put(&que_btnEvent, &btnEvent);
	}

	if(button_getState(&btn_runstop) == ACT_PUSH){
		btnEvent = BTN_RUNSTOP;
		que_put(&que_btnEvent, &btnEvent);
	}

	if(button_getState(&btn_clear) == ACT_PUSH){
		btnEvent = BTN_CLEAR;
		que_put(&que_btnEvent, &btnEvent);
	}

	if(HAL_GetTick() - prevTime >= 1000) {
		prevTime = HAL_GetTick();

		if(!que_is_empty(&que_btnEvent)){
			que_get(&que_btnEvent, &btnEvent);

			if(btnEvent == BTN_MODE){
				sound_button();
				switch(modeState){
				case TIME_CLOCK_MODE:
					modeState = TEMP_HUMID_MODE;
					break;
				case TEMP_HUMID_MODE:
					modeState = DISTANCE_MODE;
					break;
				case DISTANCE_MODE:
					modeState = STOP_WATCH_MODE;
					break;
				case STOP_WATCH_MODE:
					modeState = TIME_CLOCK_MODE;
					break;
				default:
					modeState = TIME_CLOCK_MODE;
					break;
				}
			}
			if(btnEvent == BTN_RUNSTOP){
				if(modeState == STOP_WATCH_MODE){
					runstop_Flag = true;
					StopWatch_task();
					runstop_Flag = false;
					StopWatch_task();
					sound_alarm();
				}
			}
			if(btnEvent == BTN_CLEAR){
				if(modeState == STOP_WATCH_MODE){
					clear_Flag = true;
					StopWatch_task();
					clear_Flag = false;
					StopWatch_task();
					sound_error();
				}
			}
		}
	}
}

static void smart_watch_task(void){
	switch(modeState){
	case TIME_CLOCK_MODE:
		smart_watch_TimeClockMode();
		break;
	case TEMP_HUMID_MODE:
		smart_watch_TempHumidMode();
		break;
	case DISTANCE_MODE:
		smart_watch_DistanceMode();
		break;
	case STOP_WATCH_MODE:
		smart_watch_StopWatchMode();
		break;
	default:
		smart_watch_TimeClockMode();
		break;
	}
}

static void smart_watch_TimeClockMode(void){
	TimeClock_t *pTimeClockTime = TimeClock_getTimeData();
	TimeDate_t *pTimeClockDate = TimeClock_getDateData();
	Disp_setTimeClockTime((TimeClockTime_t*) pTimeClockTime);
	Disp_setTimeClockDate((TimeClockDate_t*) pTimeClockDate);
	Disp_setMode(modeState);
	TimeClock_task();
}

static void smart_watch_TempHumidMode(void){
	TimeClock_t *pTimeClockTime = TimeClock_getTimeData();
	dht11_data_t *pTempHumid = TH_getTempHumid();
	Disp_setTimeClockTime((TimeClockTime_t*) pTimeClockTime);
	Disp_setTempHumid(pTempHumid);
	Disp_setMode(modeState);
	TH_task();
}

static void smart_watch_DistanceMode(void){
	TimeClock_t *pTimeClockTime = TimeClock_getTimeData();
	Disp_setMode(modeState);
	distance_task();
	distance_status_t status = distance_get(&dist_cm);
	if(status == DISTANCE_OK){
		Disp_setDistance(&dist_cm);
	}else if(status == DISTANCE_NO_ECHO){
		Disp_setDistanceNoEcho();
	}
	Disp_setTimeClockTime((TimeClockTime_t*) pTimeClockTime);
}

static void smart_watch_StopWatchMode(void){
	TimeClock_t *pTimeClockTime = TimeClock_getTimeData();
	StopWatch_t *pStopWatchTime = StopWatch_getTimeData();
	Disp_setTimeClockTime((TimeClockTime_t*) pTimeClockTime);
	Disp_setStopWatchTime((StopWatchTime_t*) pStopWatchTime);
	Disp_setMode(modeState);
	StopWatch_task();
}

static void uart_ISR(void){
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

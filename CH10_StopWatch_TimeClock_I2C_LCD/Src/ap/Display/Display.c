/*
 * Display.c
 *
 *  Created on: 2026. 10. 20.
 *      Author: kccistc
 */

#include "Display.h"
#include "../../driver/fnd/fnd.h"
#include "../../driver/led/led.h"
#include "../../driver/lcd/lcd.h"

// variables
led_t led_TimeClock, led_StopWatch;
led_t led_stop, led_run;
TimeClockTime_t *pTimeClockTime;
StopWatchTime_t *pStopWatchTime;
char buffer1[30];
char buffer2[30];

// function prototypes
void Disp_init(void);
void Disp_setMode(int mode);
void Disp_setStopWatchState(int state);
void Disp_setTimeClockTime(TimeClockTime_t *pTime);
void Disp_setStopWatchTime(StopWatchTime_t *pTime);
void Disp_lcdTimeClockTime(TimeClockTime_t *pTime);
void Disp_lcdStopWatchTime(StopWatchTime_t *pTime);
void Disp_lcdTimeClockTime(TimeClockTime_t *pTime);
void Disp_lcdStopWatchTime(StopWatchTime_t *pTime);
void Disp_ISR(void);
static void Disp_TimeClockTime(void);
static void Disp_StopWatchTime(void);

// function definitions
void Disp_init(void){
	led_init(&led_TimeClock, GPIOB, GPIO_PIN_15);
	led_init(&led_StopWatch, GPIOB, GPIO_PIN_14);
	led_init(&led_stop, GPIOB, GPIO_PIN_13);
	led_init(&led_run, GPIOC, GPIO_PIN_4);
	Lcd_init();
}

void Disp_setMode(int mode){
	if(mode == TIMECLOCK_MODE){
		led_on(&led_TimeClock);
		led_off(&led_StopWatch);
	}
	else {
		led_off(&led_TimeClock);
		led_on(&led_StopWatch);
	}
}

void Disp_setStopWatchState(int state){
	if (state == STOPWATCH_RUN){
		led_on(&led_stop);
		led_off(&led_run);
	}
	else {
		led_off(&led_stop);
		led_on(&led_run);
	}
}

void Disp_setTimeClockTime(TimeClockTime_t *pTime){
	pTimeClockTime = pTime;
	Disp_TimeClockTime();
}

void Disp_setStopWatchTime(StopWatchTime_t *pTime){
	pStopWatchTime = pTime;
	Disp_StopWatchTime();
}

void Disp_lcdTimeClockTime(TimeClockTime_t *pTime){
	pTimeClockTime = pTime;
	uint32_t num_hour = pTime->hour;
	uint32_t num_min = pTime->min;
	uint32_t num_sec = pTime->sec;
	uint32_t num_msec = pTime->msec;

	sprintf(buffer1, "TIME:%02d:%02d:%02d:%02d",num_hour, num_min, num_sec, num_msec);
	Lcd_writeStringXY(0, 0, buffer1);
}

void Disp_lcdStopWatchTime(StopWatchTime_t *pTime){
	pStopWatchTime = pTime;
	uint32_t num_hour = pTime->hour;
	uint32_t num_min = pTime->min;
	uint32_t num_sec = pTime->sec;
	uint32_t num_msec = pTime->msec;

	sprintf(buffer2, "STWH:%02d:%02d:%02d:%02d", num_hour, num_min, num_sec, num_msec);
	Lcd_writeStringXY(0, 1, buffer2);
}

void Disp_ISR(void){
	fnd_show_number();
}

static void Disp_TimeClockTime(void) {
	uint16_t num;
	num = (pTimeClockTime->min*100) + pTimeClockTime->sec;
	fnd_set_num(num);

	if ((pTimeClockTime->msec) < 50) {
		fnd_set_dp(FND_DIGIT_2, FND_DP_ON);
	}
	else {
		fnd_set_dp(FND_DIGIT_2, FND_DP_OFF);
	}
	fnd_set_dp(FND_DIGIT_1, FND_DP_OFF);
	fnd_set_dp(FND_DIGIT_3, FND_DP_OFF);
}

static void Disp_StopWatchTime(void) {
	uint16_t num;
	num = ((pStopWatchTime->min % 10) *1000) + (pStopWatchTime->sec*10) + (pStopWatchTime->msec / 10);
	fnd_set_num(num);

	if ((pStopWatchTime->msec) < 50) {
		fnd_set_dp(FND_DIGIT_3, FND_DP_ON);
	}
	else {
		fnd_set_dp(FND_DIGIT_3, FND_DP_OFF);
	}
	fnd_set_dp(FND_DIGIT_1, FND_DP_ON);
	fnd_set_dp(FND_DIGIT_2, FND_DP_OFF);
}



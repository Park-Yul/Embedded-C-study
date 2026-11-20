/*
 * Display.c
 *
 *  Created on: 2026. 10. 20.
 *      Author: kccistc
 */

// include
#include "Display.h"
#include "../../driver/led/led.h"
#include "../../driver/fnd/fnd.h"
#include "../../driver/lcd/lcd.h"
#include "../../driver/rtc/rtc.h"

// variables
extern I2C_HandleTypeDef hi2c1;
led_t led_TimeClock, led_StopWatch;
led_t led_stop, led_run;
TimeClockTime_t *pTimeClockTime;
TimeClockDate_t *pTimeClockDate;
StopWatchTime_t *pStopWatchTime;

// function prototypes
static void Disp_TimeClockTime(void);
static void Disp_TimeClockDate(void);
static void Disp_StopWatchTime(void);

// function definitions
void Disp_init(void){
	led_init(&led_TimeClock, GPIOB, GPIO_PIN_15);
	led_init(&led_StopWatch, GPIOB, GPIO_PIN_14);
	led_init(&led_stop, GPIOB, GPIO_PIN_13);
	led_init(&led_run, GPIOC, GPIO_PIN_4);
	Lcd_init(&hi2c1);
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
		led_off(&led_stop);
		led_on(&led_run);
	}
	else {
		led_on(&led_stop);
		led_off(&led_run);
	}
}

void Disp_setTimeClockTime(TimeClockTime_t *pTime){
	pTimeClockTime = pTime;
	Disp_TimeClockTime();
}

void Disp_setTimeClockDate(TimeClockDate_t *pDate){
	pTimeClockDate = pDate;
	Disp_TimeClockDate();
}

void Disp_setStopWatchTime(StopWatchTime_t *pTime){
	pStopWatchTime = pTime;
	Disp_StopWatchTime();
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

	char timeStr[30];
	sprintf(timeStr, "TIME:%02d:%02d:%02d", pTimeClockTime->hour, pTimeClockTime->min, pTimeClockTime->sec);
	Lcd_writeStringXY(0, 0, timeStr);
}

static void Disp_TimeClockDate(void) {
//	static TimeClockDate_t prevDate = {0};
//	if(!memcmp(&prevDate, pTimeClockDate, sizeof(TimeClockDate_t))){
//		return;
//	}
//	memcpy(&prevDate, pTimeClockDate, sizeof(TimeClockDate_t));

	char dateStr[30];
	const char *strDay[7] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};
	sprintf(dateStr, "20%02d-%02d-%02d-%s",pTimeClockDate->year, pTimeClockDate->month, pTimeClockDate->date, strDay[pTimeClockDate->day - 1]);
	Lcd_writeStringXY(0, 1, dateStr);
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

	char timeStr[30];
	sprintf(timeStr, "TIME:%02d:%02d:%02d", pTimeClockTime->hour, pTimeClockTime->min, pTimeClockTime->sec);
	Lcd_writeStringXY(0, 0, timeStr);

	char swStr[30];
	sprintf(swStr, "SW:%02d:%02d:%02d:%02d", pStopWatchTime->hour, pStopWatchTime->min, pStopWatchTime->sec, pStopWatchTime->msec);
	Lcd_writeStringXY(0, 1, swStr);
}

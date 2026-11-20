/*
 * Display.c
 *
 *  Created on: 2026. 10. 20.
 *      Author: kccistc
 */

// include
#include "Display.h"
#include "../distance/distance.h"
#include "../../../driver/led/led.h"
#include "../../../driver/fnd/fnd.h"
#include "../../../driver/lcd/lcd.h"
#include "../../../driver/rtc/rtc.h"
#include "../../../driver/dht11/dht11.h"

// define

// typedef

// variable declarations
extern I2C_HandleTypeDef hi2c1;

// variable definitions
static led_t led_TimeClock, led_TempHumid, led_Distance, led_StopWatch;
static TimeClockTime_t *pTimeClockTime;
static TimeClockDate_t *pTimeClockDate;
static StopWatchTime_t *pStopWatchTime;
static dht11_data_t *pTempHumid;
static uint16_t *pDistance;

// function declarations
static void Disp_setMode_init(void);
static void Disp_lcd_init(void);
static void Disp_TimeClockTime(void);
static void Disp_TimeClockDate(void);
static void Disp_StopWatchTime(void);
static void Disp_TempHumid(void);
static void Disp_Distance(void) ;

// function definitions
void Disp_init(void){
	led_init(&led_TimeClock, GPIOB, GPIO_PIN_15);
	led_init(&led_TempHumid, GPIOB, GPIO_PIN_14);
	led_init(&led_Distance, GPIOB, GPIO_PIN_13);
	led_init(&led_StopWatch, GPIOC, GPIO_PIN_4);
	Lcd_init(&hi2c1);
	Disp_setMode_init();
	Disp_lcd_init();
}

static void Disp_setMode_init(void){
	led_on(&led_TimeClock);
	led_on(&led_TempHumid);
	led_on(&led_Distance);
	led_on(&led_StopWatch);
}

static void Disp_lcd_init(void){
	char buff1[30], buff2[30];
	sprintf(buff1, "Welcome to");
	sprintf(buff2, "Smart Watch");
	Lcd_writeStringXY(0, 0, buff1);
	Lcd_writeStringXY(0, 1, buff2);
}

void Disp_setMode(int mode){
	static int previousMode = -1;
	if(mode == DISTANCEMODE && previousMode != DISTANCEMODE){
		Disp_setDistance(NULL);
	}
	previousMode = mode;
	switch(mode){
	case TIMECLOCK_MODE:
		led_on(&led_TimeClock);
		led_off(&led_TempHumid);
		led_off(&led_Distance);
		led_off(&led_StopWatch);
		break;
	case TEMPHUMID_MODE:
		led_off(&led_TimeClock);
		led_on(&led_TempHumid);
		led_off(&led_Distance);
		led_off(&led_StopWatch);
		break;
	case DISTANCEMODE:
		led_off(&led_TimeClock);
		led_off(&led_TempHumid);
		led_on(&led_Distance);
		led_off(&led_StopWatch);
		break;
	case STOPWATCH_MODE:
		led_off(&led_TimeClock);
		led_off(&led_TempHumid);
		led_off(&led_Distance);
		led_on(&led_StopWatch);
		break;
	default:
		led_off(&led_TimeClock);
		led_off(&led_TempHumid);
		led_off(&led_Distance);
		led_off(&led_StopWatch);
		break;
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

void Disp_setTempHumid(dht11_data_t *pTH){
	pTempHumid = pTH;
	Disp_TempHumid();
}

void Disp_setDistance(uint16_t *pDist){
	if(pDist == NULL){
		fnd_set_num(0);
		Lcd_writeStringXY(0, 1, "Distance: -- cm ");
		return;
	}
	pDistance = pDist;
	Disp_Distance();
}

void Disp_setDistanceNoEcho(void){
	fnd_set_num(0);
	Lcd_writeStringXY(0, 1, "Distance:NO ECHO");
}

void Disp_setStopWatchTime(StopWatchTime_t *pTime){
	pStopWatchTime = pTime;
	Disp_StopWatchTime();
}

static void Disp_TimeClockTime(void) {
	char timeStr[30];
	sprintf(timeStr, "TIME:%02d:%02d:%02d", pTimeClockTime->hour, pTimeClockTime->min, pTimeClockTime->sec);
	Lcd_writeStringXY(0, 0, timeStr);
}

static void Disp_TimeClockDate(void) {
	uint16_t num;
	num = (pTimeClockDate->month*100) + pTimeClockDate->date;
	fnd_set_num(num);

	fnd_set_dp(FND_DIGIT_1, FND_DP_OFF);
	fnd_set_dp(FND_DIGIT_2, FND_DP_ON);
	fnd_set_dp(FND_DIGIT_3, FND_DP_OFF);
	fnd_set_dp(FND_DIGIT_4, FND_DP_OFF);

	char dateStr[30];
	const char *strDay[7] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};
	sprintf(dateStr, "20%02d-%02d-%02d-%s    ",pTimeClockDate->year, pTimeClockDate->month, pTimeClockDate->date, strDay[pTimeClockDate->day - 1]);
	Lcd_writeStringXY(0, 1, dateStr);
}

static void Disp_TempHumid(void) {
	uint16_t num;
	num = (pTempHumid->t_int*100) + pTempHumid->t_dec;
	fnd_set_num(num);

	fnd_set_dp(FND_DIGIT_1, FND_DP_OFF);
	fnd_set_dp(FND_DIGIT_2, FND_DP_ON);
	fnd_set_dp(FND_DIGIT_3, FND_DP_OFF);
	fnd_set_dp(FND_DIGIT_4, FND_DP_OFF);

	char tempStr[30];
	sprintf(tempStr, "T:%02d.%02dC / H:%02d%%     ", pTempHumid->t_int, pTempHumid->t_dec, pTempHumid->h_int);
	Lcd_writeStringXY(0, 1, tempStr);
}

static void Disp_Distance(void) {
	uint16_t num;
	num = *pDistance;
	fnd_set_num(num);

	fnd_set_dp(FND_DIGIT_1, FND_DP_OFF);
	fnd_set_dp(FND_DIGIT_2, FND_DP_OFF);
	fnd_set_dp(FND_DIGIT_3, FND_DP_OFF);
	fnd_set_dp(FND_DIGIT_4, FND_DP_OFF);

	char distStr[17];
	snprintf(distStr, sizeof(distStr), "Dist:%5u cm   ", (unsigned int)*pDistance);
	Lcd_writeStringXY(0, 1, distStr);
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
	fnd_set_dp(FND_DIGIT_4, FND_DP_OFF);

	char swStr[30];
	sprintf(swStr, "SW:%02d:%02d:%02d:%02d    ", pStopWatchTime->hour, pStopWatchTime->min, pStopWatchTime->sec, pStopWatchTime->msec);
	Lcd_writeStringXY(0, 1, swStr);
}

void Disp_ISR(void){
	fnd_show_number();
}

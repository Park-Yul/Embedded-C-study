/*
 * TimeClock.c
 *
 *  Created on: 2026. 10. 19.
 *      Author: kccistc
 */

// include
#include "TimeClock.h"
#include "../../../driver/rtc/rtc.h"

// define

// typedef

// variables declarations
extern I2C_HandleTypeDef hi2c1;

// variables definitions
static TimeClock_t timeClockData;
static TimeDate_t timeDateData;
static ds3231_datetime_t rtc_datetime = {
		.date = {.year = 26, .month = 10, .date = 24, .day = 7},
		.time = {.hour = 18, .min =50, .sec = 30}

};
static ds3231_time_t rtc_time;
static ds3231_date_t rtc_date;

// function declarations
static void TimeClock_incMSec(void);

// function definitions
void TimeClock_init(void){
	ds3231_init(&hi2c1);
	ds3231_setDateTime(&rtc_datetime);
	ds3231_getTime(&rtc_time);
	ds3231_getDate(&rtc_date);

	timeClockData.hour = rtc_datetime.time.hour;
	timeClockData.min = rtc_datetime.time.min;
	timeClockData.sec = rtc_datetime.time.sec;
	timeClockData.msec = 0;

    timeDateData.year  = rtc_datetime.date.year;
    timeDateData.month = rtc_datetime.date.month;
    timeDateData.date  = rtc_datetime.date.date;
    timeDateData.day   = rtc_datetime.date.day;
}

void TimeClock_task(void)
{
    static uint8_t prevSec = 0xff;
    static uint8_t prevHour = 0xff;

    if (prevSec != timeClockData.sec) {
        prevSec = timeClockData.sec;

        ds3231_getTime(&rtc_time);

        timeClockData.hour = rtc_time.hour;
        timeClockData.min  = rtc_time.min;
        timeClockData.sec  = rtc_time.sec;
    }

    if (prevHour != timeClockData.hour) {
        prevHour = timeClockData.hour;

        ds3231_getDate(&rtc_date);

        timeDateData.year  = rtc_date.year;
        timeDateData.month = rtc_date.month;
        timeDateData.date  = rtc_date.date;
        timeDateData.day   = rtc_date.day;
    }
}

TimeClock_t *TimeClock_getTimeData(void){
	return &timeClockData;
}
TimeDate_t *TimeClock_getDateData(void){
	return &timeDateData;
}

void TimeClock_ISR(void) {
	TimeClock_incMSec();
}

static void TimeClock_incMSec(void)
{
	if (timeClockData.msec < 99) {
		timeClockData.msec++;
		return;
	}
	timeClockData.msec = 0;
	if(timeClockData.sec < 59) {
		timeClockData.sec++;
		return;
	}
	timeClockData.sec = 0;
	if(timeClockData.min < 59) {
		timeClockData.min++;
		return;
	}
	timeClockData.min = 0;
	if(timeClockData.hour < 23) {
		timeClockData.hour++;
		return;
	}
	timeClockData.hour = 0;
}

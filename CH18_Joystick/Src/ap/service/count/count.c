/*
 * count.c
 *
 *  Created on: 2026. 11. 3.
 *      Author: kccistc
 */

// include
#include "count.h"
#include "../../../driver/joystick/joystick.h"

// define
#define ADC_CENTER   506
#define JOY_BOT      5
#define JOY_TOP      1023
#define JOY_DEAD     10

#define sp_fast      100
#define sp_slow      1000

// typedef

// variable declarations
extern ADC_HandleTypeDef hadc1;


// variable definitions
static joystick_data_t joydata;

static int32_t xcount = 0;
static int32_t ycount = 0;

static uint32_t xspeed = 0;
static uint32_t yspeed = 0;

static uint32_t prevxtime = 0;
static uint32_t prevytime = 0;

static int8_t prevxdir = 0;
static int8_t prevydir = 0;

// function declarations

// function definitions
void count_init(void){
	joystick_init(&hadc1);
	joydata.x = ADC_CENTER;
	joydata.y = ADC_CENTER;
}

void count_task(void){
	uint32_t now = HAL_GetTick();

	int32_t x = 0, y = 0;
	int32_t xamount = 0, yamount = 0;
	int32_t xmax = 0, ymax = 0;

	int8_t xdir = 0;
	int8_t ydir = 0;

	if (joystick_read(&joydata) == HAL_OK)
	{
		x = joydata.x - ADC_CENTER;
		y = joydata.y - ADC_CENTER;

		if (x > JOY_DEAD)
		{
			xdir = 1;
		}
		else if (x < -JOY_DEAD)
		{
			xdir = -1;
		}

		if (y > JOY_DEAD)
		{
			ydir = 1;
		}
		else if (y < -JOY_DEAD)
		{
			ydir = -1;
		}

		xamount = x;
		yamount = y;

		if (xamount < 0)
		{
			xamount = -xamount;
		}

		if (yamount < 0)
		{
			yamount = -yamount;
		}

		if (xdir == 0)
		{
			xspeed = 0;
		}
		else
		{
			if (xdir > 0)
			{
				xmax = JOY_TOP - ADC_CENTER;
			}
			else
			{
				xmax = ADC_CENTER - JOY_BOT;
			}

			if (xamount > xmax)
			{
				xamount = xmax;
			}

			xspeed = sp_slow -
					(xamount - JOY_DEAD) * (sp_slow - sp_fast)
					/ (xmax - JOY_DEAD);
		}

		if (ydir == 0)
		{
			yspeed = 0;
		}
		else
		{
			if (ydir > 0)
			{
				ymax = JOY_TOP - ADC_CENTER;
			}
			else
			{
				ymax = ADC_CENTER - JOY_BOT;
			}

			if (yamount > ymax)
			{
				yamount = ymax;
			}

			yspeed = sp_slow -
					(yamount - JOY_DEAD) * (sp_slow - sp_fast)
					/ (ymax - JOY_DEAD);
		}

		if (xdir != prevxdir)
		{
			prevxdir = xdir;
			prevxtime = now - xspeed;
		}

		if (ydir != prevydir)
		{
			prevydir = ydir;
			prevytime = now - yspeed;
		}

		if (xdir != 0 && now - prevxtime >= xspeed)
		{
			if (xdir == 1 && xcount < 9999)
			{
				xcount++;
			}
			else if (xdir == -1 && xcount > -9999)
			{
				xcount--;
			}

			prevxtime = now;
		}

		if (ydir != 0 && now - prevytime >= yspeed)
		{
			if (ydir == 1 && ycount < 9999)
			{
				ycount++;
			}
			else if (ydir == -1 && ycount > -9999)
			{
				ycount--;
			}

			prevytime = now;
		}
	}
}

int32_t getXcount(void){
	return xcount;
}


int32_t getYcount(void){
	return ycount;
}


void count_ISR(void){
	joystick_ISR();
}



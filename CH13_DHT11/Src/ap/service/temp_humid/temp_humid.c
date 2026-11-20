/*
 * temp_humid.c
 *
 *  Created on: 2026. 10. 28.
 *      Author: kccistc
 */

// include
#include "temp_humid.h"
#include "stm32f4xx_hal.h"

// variable declarations

// variable definitions
static dht11_data_t TH_data;

// function declarations

// function definitions
void TH_init(void){
	dht11_init(DHT_PORT, DHT_PIN);
}

void TH_task(void){
	static uint32_t prevTime = 0;
	if(HAL_GetTick() - prevTime <2000) {
		return;
	}
	prevTime = HAL_GetTick();
	dht11_read(&TH_data);
}

void TH_getTempHumid(dht11_data_t *tempHumid){
	*tempHumid = TH_data;
}

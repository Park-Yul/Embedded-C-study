/*
 * temp_humid.c
 *
 *  Created on: 2026. 10. 28.
 *      Author: kccistc
 */

// include
#include "temp_humid.h"
#include "../../../driver/dht11/dht11.h"

// define

// typedef

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
	if(HAL_GetTick() - prevTime <1000) {
		return;
	}
	prevTime = HAL_GetTick();
	dht11_read(&TH_data);
}

dht11_data_t *TH_getTempHumid(void){
	return &TH_data;
}

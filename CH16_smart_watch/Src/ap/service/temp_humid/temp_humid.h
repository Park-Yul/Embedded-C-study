/*
 * temp_humid.h
 *
 *  Created on: 2026. 10. 28.
 *      Author: kccistc
 */

#ifndef SRC_AP_SERVICE_TEMP_HUMID_TEMP_HUMID_H_
#define SRC_AP_SERVICE_TEMP_HUMID_TEMP_HUMID_H_

// include
#include "stm32f4xx_hal.h"
#include "../../../driver/dht11/dht11.h"

// define
#define DHT_PORT 	GPIOC
#define DHT_PIN		GPIO_PIN_6

// typedef

// variable declarations

// function declarations
void TH_init(void);
void TH_task(void);
dht11_data_t *TH_getTempHumid(void);

#endif /* SRC_AP_SERVICE_TEMP_HUMID_TEMP_HUMID_H_ */

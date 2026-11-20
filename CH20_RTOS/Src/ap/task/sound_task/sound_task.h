/*
 * sound.h
 *
 *  Created on: 2026. 10. 30.
 *      Author: kccistc
 */

#ifndef SRC_AP_SOUND_SOUND_H_
#define SRC_AP_SOUND_SOUND_H_

// include
#include "stm32f4xx_hal.h"
#include <stdbool.h>

// define

// typedef

// variable declarations
extern bool sound_flag;

// function declarations
void sound_task_init(void);
void sound_task(void);


#endif /* SRC_AP_SOUND_SOUND_H_ */

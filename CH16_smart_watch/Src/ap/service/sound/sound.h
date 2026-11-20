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

// define

// typedef

// variable declarations

// function declarations
void sound_init(void);
void sound_button(void);
void sound_error(void);
void sound_alarm(void);

#endif /* SRC_AP_SOUND_SOUND_H_ */

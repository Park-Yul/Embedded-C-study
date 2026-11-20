/*
 * fnd.h
 *
 *  Created on: 2026. 10. 14.
 *      Author: kccistc
 */

#ifndef SRC_DRIVER_FND_FND_H_
#define SRC_DRIVER_FND_FND_H_

// include
#include "stm32f4xx_hal.h"

// define
#define FND_DIGIT_1		1
#define FND_DIGIT_2		2
#define FND_DIGIT_3		3
#define FND_DIGIT_4		4

#define FND_DP_OFF		0
#define FND_DP_ON		1

#define SEG_DP_OFF		11
#define	SEG_DP_ON		10

// typedef
typedef struct {
	GPIO_TypeDef* GPIOx;
	uint16_t GPIO_Pin;
} set_port_t;

// function prototypes
void fnd_set_dp(int dig, uint16_t state);
uint16_t fnd_get_dp(void);
void fnd_set_num(uint16_t n);
uint16_t fnd_get_num(void);
void fnd_show_number(void);

#endif /* SRC_DRIVER_FND_FND_H_ */

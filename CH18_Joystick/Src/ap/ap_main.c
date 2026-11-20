/*
 * ap_main.c
 *
 *  Created on: 2026. 11. 3.
 *      Author: kccistc
 */

// include
#include "ap_main.h"
#include "service/count/count.h"
#include "service/display/display.h"

// define

// typedef

// variable declarations

// variable definitions

// function declarations
static void ap_init(void);


// function definitions
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc){
	count_ISR();
}

int ap_main(void){
	int32_t xcount;
	int32_t ycount;
	ap_init();
	while(1){
		count_task();
		xcount = getXcount();
		ycount = getYcount();
		disp_task(xcount, ycount);
	}
}

static void ap_init(void){
	count_init();
	disp_init();
}


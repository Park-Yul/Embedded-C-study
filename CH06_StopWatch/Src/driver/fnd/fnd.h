/*
 * fnd.h
 *
 *  Created on: 2026. 10. 14.
 *      Author: kccistc
 */

#ifndef SRC_DRIVER_FND_FND_H_
#define SRC_DRIVER_FND_FND_H_

#include "stm32f4xx_hal.h"

void fnd_stop(void);
void fnd_up(void);
void fnd_down(void);
void fnd_clear(void);
int runstop_edge(int *flag, int button_input);
int clear_edge(int *flag, int button_input);
int mode_edge(int *flag, int button_input);

#endif /* SRC_DRIVER_FND_FND_H_ */

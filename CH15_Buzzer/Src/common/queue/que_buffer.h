/*
 * que_buffer.h
 *
 *  Created on: 2026. 10. 30.
 *      Author: kccistc
 */

#ifndef SRC_COMMON_QUEUE_QUE_BUFFER_H_
#define SRC_COMMON_QUEUE_QUE_BUFFER_H_

// include
#include "stm32f4xx_hal.h"
#include <stdbool.h>
#include <string.h>

// define
#define BTN_EVENT_Q_SIZE	6

// typedef
typedef struct{
	uint8_t *buf;
	uint16_t elem_size;
	uint16_t capacity;
	volatile uint16_t head;
	volatile uint16_t tail;
} que_buf_t;

typedef enum{
	BTN_NORMAL = 0,
	BTN_ERROR,
	BTN_ALARM
}btn_event_t;

// variable declarations

// function declarations
void que_buf_init(que_buf_t *que, void *storage, uint16_t elem_size, uint16_t capacity);
bool que_put(que_buf_t *que, void *elem);
bool que_get(que_buf_t *que, void *elem);
bool que_is_empty(que_buf_t *que);
bool que_is_full(que_buf_t *que);

#endif /* SRC_COMMON_QUEUE_QUE_BUFFER_H_ */

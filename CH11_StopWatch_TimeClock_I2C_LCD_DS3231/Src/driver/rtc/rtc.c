/*
 * rtc.c
 *
 *  Created on: 2026. 10. 21.
 *      Author: kccistc
 */

// include
#include "rtc.h"

// variables
I2C_HandleTypeDef *ds3231_hi2c;

// function prototypes
static uint8_t bcd2bin(uint8_t v);
static uint8_t bin2bcd(uint8_t v);

// function definitions
void ds3231_init(I2C_HandleTypeDef *hi2c){
	ds3231_hi2c = hi2c;
}

void ds3231_getTime(ds3231_time_t *t){
	uint8_t buf[3];
	uint8_t ds3231_regAddr = DS3231_REG_SEC;
//	HAL_I2C_Master_Transmit(&hi2c1, DS3231_I2C_ADDR, &ds3231_regAddr, 1, DS3231_TIMEOUT_MS);
//	HAL_I2C_Master_Receive(&hi2c1, DS3231_I2C_ADDR, buf, sizeof(buf), DS3231_TIMEOUT_MS);
	HAL_I2C_Mem_Read(ds3231_hi2c, DS3231_I2C_ADDR, ds3231_regAddr, 1, buf, sizeof(buf), DS3231_TIMEOUT_MS);
	t->sec = bcd2bin(buf[0] & 0x7F);
	t->min = bcd2bin(buf[1] & 0x7F);
	t->hour = bcd2bin(buf[2] & 0x7F);
}

void ds3231_setTime(const ds3231_time_t *t){
	uint8_t buf[4];
	buf[0] = DS3231_REG_SEC;
	buf[1] = bin2bcd(t->sec);
	buf[2] = bin2bcd(t->min);
	buf[3] = bin2bcd(t->hour);
//	HAL_I2C_Master_Transmit(&hi2c1, DS3231_I2C_ADDR, buf, sizeof(buf) , DS3231_TIMEOUT_MS);
	HAL_I2C_Mem_Write(ds3231_hi2c, DS3231_I2C_ADDR, buf[0], 1, &buf[1], sizeof(buf)-sizeof(uint8_t), DS3231_TIMEOUT_MS);
}

void ds3231_getDate(ds3231_date_t *d){
	uint8_t buf[4];
	uint8_t ds3231_regAddr = DS3231_REG_DAY;
//	HAL_I2C_Master_Transmit(&hi2c1, DS3231_I2C_ADDR, &ds3231_regAddr, 1, DS3231_TIMEOUT_MS);
//	HAL_I2C_Master_Receive(&hi2c1, DS3231_I2C_ADDR, buf, sizeof(buf), DS3231_TIMEOUT_MS);
	HAL_I2C_Mem_Read(ds3231_hi2c, DS3231_I2C_ADDR, ds3231_regAddr, 1, buf, sizeof(buf), DS3231_TIMEOUT_MS);
	d->day = bcd2bin(buf[0] & 0x0E);
	d->date = bcd2bin(buf[1] & 0x3F);
	d->month = bcd2bin(buf[2] & 0x1F);
	d->year = bcd2bin(buf[3]);
}

void ds3231_setDate(const ds3231_date_t *d){
	uint8_t buf[5];
	buf[0] = DS3231_REG_DAY;
	buf[1] = bin2bcd(d->day);
	buf[2] = bin2bcd(d->date);
	buf[3] = bin2bcd(d->month);
	buf[4] = bin2bcd(d->year);
//	HAL_I2C_Master_Transmit(&hi2c1, DS3231_I2C_ADDR, buf, sizeof(buf), DS3231_TIMEOUT_MS);
	HAL_I2C_Mem_Write(ds3231_hi2c, DS3231_I2C_ADDR, buf[0], 1, buf+1, sizeof(buf)-sizeof(uint8_t), DS3231_TIMEOUT_MS);
}

void ds3231_getDateTime(ds3231_datetime_t *dt){
	uint8_t buf[7];
	uint8_t ds3231_regAddr = DS3231_REG_SEC;
//	HAL_I2C_Master_Transmit(&hi2c1, DS3231_I2C_ADDR, &ds3231_regAddr, 1, DS3231_TIMEOUT_MS);
//	HAL_I2C_Master_Receive(&hi2c1, DS3231_I2C_ADDR, buf, sizeof(buf), DS3231_TIMEOUT_MS);
	HAL_I2C_Mem_Read(ds3231_hi2c, DS3231_I2C_ADDR, ds3231_regAddr, 1, buf, sizeof(buf), DS3231_TIMEOUT_MS);
	dt->time.sec = bcd2bin(buf[0] & 0x7F);
	dt->time.min = bcd2bin(buf[1] & 0x7F);
	dt->time.hour = bcd2bin(buf[2] & 0x7F);
	dt->date.day = bcd2bin(buf[3] & 0x0F);
	dt->date.date = bcd2bin(buf[4] & 0x3F);
	dt->date.month = bcd2bin(buf[5] & 0x1F);
	dt->date.year = bcd2bin(buf[6]);
}

void ds3231_setDateTime(const ds3231_datetime_t *dt){
	uint8_t buf[8];
	buf[0] = DS3231_REG_SEC;
	buf[1] = bin2bcd(dt->time.sec);
	buf[2] = bin2bcd(dt->time.min);
	buf[3] = bin2bcd(dt->time.hour);
	buf[4] = bin2bcd(dt->date.day);
	buf[5] = bin2bcd(dt->date.date);
	buf[6] = bin2bcd(dt->date.month);
	buf[7] = bin2bcd(dt->date.year);
//	HAL_I2C_Master_Transmit(&hi2c1, DS3231_I2C_ADDR, buf, sizeof(buf) , DS3231_TIMEOUT_MS);
	HAL_I2C_Mem_Write(ds3231_hi2c, DS3231_I2C_ADDR, buf[0], 1, buf+1, sizeof(buf)-sizeof(uint8_t), DS3231_TIMEOUT_MS);
}

static uint8_t bcd2bin(uint8_t v){
	return ((v >> 4) * 10) + (v & 0x0f);
}

static uint8_t bin2bcd(uint8_t v){
	return ((v / 10) << 4 ) | (v % 10);
}

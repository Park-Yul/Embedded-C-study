/*
 * sound.c
 *
 *  Created on: 2026. 10. 30.
 *      Author: kccistc
 */

// include
#include "sound.h"
#include "../../../driver/buzzer/buzzer.h"

// define
/* Frequencies in Hz; S denotes sharp (#). */
/* Octave 1 */
#define C1   33
#define CS1  35
#define D1   37
#define DS1  39
#define E1   41
#define F1   44
#define FS1  46
#define G1   49
#define GS1  52
#define A1   55
#define AS1  58
#define B1   62

/* Octave 2 */
#define C2   65
#define CS2  69
#define D2   73
#define DS2  78
#define E2   82
#define F2   87
#define FS2  92
#define G2   98
#define GS2  104
#define A2   110
#define AS2  117
#define B2   123

/* Octave 3 */
#define C3   131
#define CS3  139
#define D3   147
#define DS3  156
#define E3   165
#define F3   175
#define FS3  185
#define G3   196
#define GS3  208
#define A3   220
#define AS3  233
#define B3   247

/* Octave 4 */
#define C4   262
#define CS4  277
#define D4   294
#define DS4  311
#define E4   330
#define F4   349
#define FS4  370
#define G4   392
#define GS4  415
#define A4   440
#define AS4  466
#define B4   494

/* Octave 5 */
#define C5   523
#define CS5  554
#define D5   587
#define DS5  622
#define E5   659
#define F5   698
#define FS5  740
#define G5   784
#define GS5  831
#define A5   880
#define AS5  932
#define B5   988

/* Octave 6 */
#define C6   1047
#define CS6  1109
#define D6   1175
#define DS6  1245
#define E6   1319
#define F6   1397
#define FS6  1480
#define G6   1568
#define GS6  1661
#define A6   1760
#define AS6  1865
#define B6   1976

/* Octave 7 */
#define C7   2093
#define CS7  2217
#define D7   2349
#define DS7  2489
#define E7   2637
#define F7   2794
#define FS7  2960
#define G7   3136
#define GS7  3322
#define A7   3520
#define AS7  3729
#define B7   3951

/* Octave 8 */
#define C8   4186
#define CS8  4435
#define D8   4699
#define DS8  4978
#define E8   5274
#define F8   5588
#define FS8  5920
#define G8   6272
#define GS8  6645
#define A8   7040
#define AS8  7459
#define B8   7902

/* bpm */
#define BPM		125


// typedef

// variable declarations
extern TIM_HandleTypeDef htim1;

// variable definitions
uint32_t four;
uint32_t two;
uint32_t one;
uint32_t eight;
uint32_t sixteen;

// function declarations

// function definitions
void sound_init(void){
	four = 60000 / BPM;
	two = four * 2;
	one = two * 2;
	eight = four / 2;
	sixteen = eight / 2;
	buzzer_init(&htim1, TIM_CHANNEL_4);

}

void sound_power(void){
//	buzzer_on(C6);
//	HAL_Delay(250);
//	buzzer_on(E6);
//	HAL_Delay(250);
//	buzzer_on(G6);
//	HAL_Delay(250);
//	buzzer_on(C7);
//	HAL_Delay(250);
//	buzzer_off();
	buzzer_on(E5);
	HAL_Delay(sixteen);
	buzzer_off();
	HAL_Delay(10);

	buzzer_on(E5);
	HAL_Delay(sixteen);

	buzzer_off();
	HAL_Delay(sixteen);

	buzzer_on(E5);
	HAL_Delay(sixteen);

	buzzer_off();
	HAL_Delay(sixteen);

	buzzer_on(C5);
	HAL_Delay(sixteen);
	buzzer_off();
	HAL_Delay(10);

	buzzer_on(E5);
	HAL_Delay(sixteen);

	buzzer_off();
	HAL_Delay(sixteen);

	buzzer_on(G5);
	HAL_Delay(sixteen);

	buzzer_off();
}

void sound_button(void){
	buzzer_on(G6);
	HAL_Delay(40);
	buzzer_on(C7);
	HAL_Delay(60);
	buzzer_off();
}

void sound_error(void){
	buzzer_on(C5);
	HAL_Delay(150);
	buzzer_off();
	HAL_Delay(60);
	buzzer_on(C5);
	HAL_Delay(150);
	buzzer_off();
}

void sound_alarm(void){
	buzzer_on(C7);
	HAL_Delay(50);
	buzzer_off();
	HAL_Delay(50);
	buzzer_on(C7);
	HAL_Delay(50);
	buzzer_off();
	HAL_Delay(50);
	buzzer_on(C7);
	HAL_Delay(50);
	buzzer_off();
	HAL_Delay(50);
	buzzer_on(C7);
	HAL_Delay(50);
	buzzer_off();
	HAL_Delay(50);
}

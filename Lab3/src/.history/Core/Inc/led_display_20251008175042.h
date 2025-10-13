/*
 * led_display.h
 *
 *  Created on: Oct 2, 2025
 *      Author: ADMIN
 */

#ifndef INC_LED_DISPLAY_H_
#define INC_LED_DISPLAY_H_

#include "main.h"

#define MAX_LED 4
#define OFF 10 // Off value for 7-segment display

// Trạng thái đèn
#define LED_OFF 0
#define LED_ON  1

// Màu đèn
#define RED     0
#define YELLOW  1
#define GREEN   2
#define ALL_LED 3
#define NONE    4

extern int led_buffer[4];
extern int index_led;
extern int led_status;
extern int redDuration;
extern int yellowDuration;
extern int greenDuration;
extern int horLightState;
extern int verLightState;

void display7SEG_V(int num);
void display7SEG_H(int num);
void update7SEG(int index);
void clearAllTrafficLights(void);
void setTrafficLight(int road, int color);
void toggleTrafficLight(int color);
void updateLedBuffer(int hor_value, int ver_value);

#endif /* INC_LED_DISPLAY_H_ */

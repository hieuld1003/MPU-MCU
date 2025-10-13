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

extern int led_buffer[4];
extern int index_led;
extern int led_status;
extern int redDuration;
extern int yellowDuration;
extern int greenDuration;

void display7SEG(int num);
void display7SEG_V(int num);
void display7SEG_H(int num);
void update7SEG(int index);
void updateLedBuffer(void);
void clearAllTrafficLights(void);
void setColorLights(int hLight, int vLight);
void blinkAllLights(int colorType);

#endif /* INC_LED_DISPLAY_H_ */

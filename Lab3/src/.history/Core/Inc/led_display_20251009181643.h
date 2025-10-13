/*
 * led_display.h
 *
 *  Created on: Oct 2, 2025
 *      Author: ADMIN
 */

#ifndef INC_LED_DISPLAY_H_
#define INC_LED_DISPLAY_H_

#include "main.h"

extern int led_buffer[4];

void updateLedBuffer(int duration_h, int duration_v);
void scan7SEG();

void set_red_h(int state);
void set_yellow_h(int state);
void set_green_h(int state);

void set_red_v(int state);
void set_yellow_v(int state);
void set_green_v(int state);

#endif /* INC_LED_DISPLAY_H_ */
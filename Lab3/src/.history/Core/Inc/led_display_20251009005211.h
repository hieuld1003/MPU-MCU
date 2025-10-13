/*
 * led_display.h
 *
 *  Created on: Oct 2, 2025
 *      Author: ADMIN
 */

#ifndef INC_LED_DISPLAY_H_
#define INC_LED_DISPLAY_H_

#include "main.h"

void update7SEG(int index);
void updateLedBuffer(int duration1, int duration2);
void setTrafficLight(int status);
void scan7SEG();

#endif /* INC_LED_DISPLAY_H_ */
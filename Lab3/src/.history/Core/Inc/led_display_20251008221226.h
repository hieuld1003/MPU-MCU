/*
 * led_display.h
 *
 *  Created on: Oct 2, 2025
 *      Author: ADMIN
 */

#ifndef INC_LED_DISPLAY_H_
#define INC_LED_DISPLAY_H_

void display7SEG(int num);
void update_seven_segment_led(void);
void update_clock_buffer(int index, int num);
void clearAllLed(void);

extern int led_buffer[4];
extern int led_index;

#endif /* INC_LED_DISPLAY_H_ */

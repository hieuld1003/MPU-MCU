/*
 * exercise_1.h
 *
 *  Created on: Sep 12, 2025
 *      Author: Luong Duc Hieu
 */

#ifndef SRC_EX1_H_
#define SRC_EX1_H_
#include "main.h"

unsigned int setInit = 0;
unsigned int count = 0;

void toggleLED() {
	HAL_GPIO_TogglePin(red_led_GPIO_Port, red_led_Pin);
	HAL_GPIO_TogglePin(yellow_led_GPIO_Port, yellow_led_Pin);
}

void setInitLED() {
	HAL_GPIO_WritePin(red_led_GPIO_Port, red_led_Pin, 0);
	HAL_GPIO_WritePin(yellow_led_GPIO_Port, yellow_led_Pin, 1);
}

void exercise_1() {
	if (setInit == 0) {
		setInitLED();
		setInit ++;
	}
	if (count == 1) {
		count = 0;
		toggleLED();
	}
	else ++count;
}


#endif /* SRC_EX1_H_ */

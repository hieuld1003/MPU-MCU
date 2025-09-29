/*
 * exercise_2.h
 *
 *  Created on: Sep 12, 2025
 *      Author: Luong Duc Hieu
 */

#ifndef SRC_EX2_H_
#define SRC_EX2_H_
#include "main.h"

unsigned int init = 0;
unsigned int count = 0;
unsigned int phase = 0;

void initial() {
	HAL_GPIO_WritePin(red_led_GPIO_Port, red_led_Pin, 0);
	HAL_GPIO_WritePin(yellow_led_GPIO_Port, yellow_led_Pin, 1);
	HAL_GPIO_WritePin(green_led_GPIO_Port, green_led_Pin, 1);
}

void Phase2() {
	// Switch red light to off, switch yellow light to on
	HAL_GPIO_TogglePin(red_led_GPIO_Port, red_led_Pin);
	HAL_GPIO_TogglePin(yellow_led_GPIO_Port, yellow_led_Pin);

}

void Phase3() {
	//Switch yellow to off; switch green to on
	HAL_GPIO_TogglePin(yellow_led_GPIO_Port, yellow_led_Pin);
	HAL_GPIO_TogglePin(green_led_GPIO_Port, green_led_Pin);
}

void Phase1() {
	//Switch green to off; switch red to on;
	HAL_GPIO_TogglePin(green_led_GPIO_Port, green_led_Pin);
	HAL_GPIO_TogglePin(red_led_GPIO_Port, red_led_Pin);
}


void exercise_2() {
	if (init == 0){
		initial();
		phase = 1;
		init++;
	}

	switch (phase){
		case 1:
		{
			if (count == 5){
				phase = 2;
				Phase2();
				count = 0;
			}
			break;
		}
		case 2:
		{
			if (count == 3){
				phase = 3;
				Phase3();
				count = 0;
			}
			break;
		}
		case 3:
			{
			if (count == 2){
				phase = 1;
				Phase1();
				count = 0;
			}
			break;
		}
	}
	++count;
}

#endif /* SRC_EXERCISE_2_H_ */

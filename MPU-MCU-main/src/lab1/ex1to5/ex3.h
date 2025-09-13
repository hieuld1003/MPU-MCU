/*
 * exercise_3.h
 *
 *  Created on: Sep 12, 2025
 *      Author: Luong Duc Hieu
 */

#ifndef SRC_EX3_H_
#define SRC_EX3_H_

#include "main.h"


int init = 0;

/****************************************************/
//Vertical LED:
//GREEN - 3 sec
//YELLOW - 2 sec
//RED - 5 sec
//
//Horizontal LED:
//RED - 5 sec
//GREEN - 3 sec
//YELLOW - 2 sec

/****************************************************/

int ver_phase = 0, hor_phase = 0;
int ver_count = 0, hor_count = 0;

//Vertical LED:
void ver_setup_init(){
	ver_phase = 1;
	HAL_GPIO_WritePin(red_v_GPIO_Port, red_v_Pin, 0);
	HAL_GPIO_WritePin(yellow_v_GPIO_Port, yellow_v_Pin, 0);
	HAL_GPIO_WritePin(green_v_GPIO_Port, green_v_Pin, 1);
}

void ver_phase2(){
	ver_phase = 2;
	HAL_GPIO_TogglePin(yellow_v_GPIO_Port, yellow_v_Pin);
	HAL_GPIO_TogglePin(green_v_GPIO_Port, green_v_Pin);
}

void ver_phase3(){
	ver_phase = 3;
//	Turn on: RED
	HAL_GPIO_TogglePin(red_v_GPIO_Port, red_v_Pin);
//	Turn off: yellow
	HAL_GPIO_TogglePin(yellow_v_GPIO_Port, yellow_v_Pin);
}
void ver_phase1(){
	ver_phase = 1;
//	Turn on: green
	HAL_GPIO_TogglePin(green_v_GPIO_Port, green_v_Pin);
//	Turn off: RED
	HAL_GPIO_TogglePin(red_v_GPIO_Port, red_v_Pin);
}


//Horizontal LED:
void hor_setup_init(){
	HAL_GPIO_WritePin(red_h_GPIO_Port, red_h_Pin, 1);
	HAL_GPIO_WritePin(yellow_h_GPIO_Port, yellow_h_Pin, 0);
	HAL_GPIO_WritePin(green_h_GPIO_Port, green_h_Pin, 0);
	hor_phase = 1;
}
void hor_phase2(){
	hor_phase = 2;
//	Turn on: green
	HAL_GPIO_TogglePin(green_h_GPIO_Port, green_h_Pin);
//	Turn off: RED
	HAL_GPIO_TogglePin(red_h_GPIO_Port, red_h_Pin);
}

void hor_phase3(){
	hor_phase = 3;
//	Turn on: yellow
	HAL_GPIO_TogglePin(yellow_h_GPIO_Port, yellow_h_Pin);
//	Turn off: green
	HAL_GPIO_TogglePin(green_h_GPIO_Port, green_h_Pin);
}

void hor_phase1(){
	hor_phase = 1;
//	Turn on: RED
	HAL_GPIO_TogglePin(red_h_GPIO_Port, red_h_Pin);
//	Turn off: yellow
	HAL_GPIO_TogglePin(yellow_h_GPIO_Port, yellow_h_Pin);
}

void exercise_3(){
	if (init == 0){
		++init;
		ver_setup_init();
		hor_setup_init();
	}

	switch(ver_phase){
	case 1:
		if (ver_count >= 3) {
			ver_phase2();
			ver_count = 0;
		}
		break;
	case 2:
		if (ver_count >= 2) {
			ver_phase3();
			ver_count = 0;
		}
		break;
	case 3:
		if (ver_count >= 5) {
			ver_phase1();
			ver_count = 0;
		}
		break;
	}

	switch(hor_phase){
	case 1:
		if (hor_count >= 5) {
			hor_phase2();
			hor_count = 0;
		}
		break;
	case 2:
		if (hor_count >= 3) {
			hor_phase3();
			hor_count = 0;
		}
		break;
	case 3:
		if (hor_count >= 2) {
			hor_phase1();
			hor_count = 0;
		}
		break;
	}
	++ver_count;
	++hor_count;
}


#endif /* SRC_EXERCISE_3_H_ */

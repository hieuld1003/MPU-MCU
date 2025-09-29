/*
 * exercise_5.h
 *
 *  Created on: Sep 12, 2025
 *      Author: Luong Duc Hieu
 */

#ifndef SRC_EXERCISE_5_H_
#define SRC_EXERCISE_5_H_

#include "ex3.h"
#include "ex4.h"
#include "main.h"

void display7SEG_2(int num){
	if (num >= 0 && num <= 9){
		for (int state = 7; state < 14; state++){
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0 << state, arr[num - 1][state]);
		}
	}
}
void exercise_5(){
	exercise_3();

	switch(ver_phase){
	case 1:
		display7SEG(3 - ver_count);
		break;
	case 2:
		display7SEG(2 - ver_count);
		break;
	case 3:
		display7SEG(5 - ver_count);
		break;
	}
	switch (hor_phase){
	case 1:
		display7SEG_2(5 - hor_count);
		break;
	case 2:
		display7SEG_2(3 - hor_count);
		break;
	case 3:
		display7SEG_2(2 - hor_count);
		break;
	}
}


#endif /* SRC_EXERCISE_5_H_ */

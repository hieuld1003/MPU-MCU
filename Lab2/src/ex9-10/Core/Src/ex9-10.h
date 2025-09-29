/*
 * ex9-10.h
 *
 *  Created on: Sep 23, 2025
 *      Author: ADMIN
 */

#ifndef SRC_EX9_10_H_
#define SRC_EX9_10_H_
#include "ex9-10.h"

int matrix_idx = 0;
uint8_t matrix_buffer_A[8] = {0xE7, 0xDB, 0xBD, 0xBD, 0x81, 0xBD, 0xBD, 0xBD};//A
//uint8_t matrix_buffer_A[8] = {0x81, 0xBF, 0xBF, 0x81, 0xBF, 0xBF, 0x81, 0xFF};//E

/* TIMER STARTS HERE */
int counter = 0, flag = 0;

int const cycle = 1;

void set(int duration){
	counter = duration / cycle;
	flag = 0;
}

void run(void){
	counter--;
	if (counter == 0) flag = 1;
}

/* TIMER ENDS HERE */
int convertedBinary[8] = {0,0,0,0,0,0,0,0};

void convertToBinary(uint8_t num) {
    for(int i = 7; i >= 0; i--) {
        convertedBinary[i] = num % 2;
        num = num / 2;
    }
}

void shiftLeft(){
    for (int i = 0; i < 8; i++) {
        matrix_buffer_A[i] = (matrix_buffer_A[i] << 1) | (matrix_buffer_A[i] >> 7);
    }
}


void updateLEDMatrix(int index){
	uint16_t enm_pins[] = {ENM0_Pin, ENM1_Pin, ENM2_Pin, ENM3_Pin, ENM4_Pin, ENM5_Pin, ENM6_Pin, ENM7_Pin};
    if (index < 8) {
        convertToBinary(matrix_buffer_A[index]);
        for(int i = 0; i < 8; i++){
        	HAL_GPIO_WritePin(GPIOA, enm_pins[i], (convertedBinary[i] == 0) ? GPIO_PIN_RESET : GPIO_PIN_SET);
        }
    }
}
void resetState(){
    HAL_GPIO_WritePin(GPIOB, ROW0_Pin|ROW1_Pin|ROW2_Pin|ROW3_Pin|ROW4_Pin|ROW5_Pin|ROW6_Pin|ROW7_Pin, GPIO_PIN_SET);
}
void displayLEDMatrix(){
	if (matrix_idx >= 8) {
	        matrix_idx = 0;
//	        shiftLeft();
	    }
	    resetState();
	    HAL_GPIO_WritePin(GPIOB, ROW0_Pin << matrix_idx, GPIO_PIN_RESET);
	    updateLEDMatrix(matrix_idx);
}

#endif /* SRC_EX9_10_H_ */

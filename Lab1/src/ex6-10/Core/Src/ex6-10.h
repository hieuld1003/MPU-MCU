/*
 * ex6-10.h
 *
 *  Created on: Sep 13, 2025
 *      Author: Luong Duc Hieu
 */

#ifndef SRC_EX6_10_H_
#define SRC_EX6_10_H_

#include "main.h"


uint16_t clockPins[12] = {
    d12_Pin, d1_Pin, d2_Pin, d3_Pin, d4_Pin, d5_Pin,
    d6_Pin, d7_Pin, d8_Pin, d9_Pin, d10_Pin, d11_Pin
};

void testLEDs() {
    for (int i = 0; i < 12; i++) {
        HAL_GPIO_WritePin(GPIOA, clockPins[i], 1);
        HAL_Delay(500);
    }
}

void clearAllClock() {
    for (int i = 0; i < 12; i++) {
        HAL_GPIO_WritePin(GPIOA, clockPins[i], 0);
    }
}

void setNumberOnClock(int num) {
    if (num >= 0 && num < 12) {
        HAL_GPIO_WritePin(GPIOA, clockPins[num], 1);
    }
}

void clearNumberOnClock(int num) {
    if (num >= 0 && num < 12) {
        HAL_GPIO_WritePin(GPIOA, clockPins[num], 0);
    }
}


int second = 0,
	minute = 0,
	hour = 0;
void displayClock() {

    ++second;
    if (second > 59){
        second = 0;
        ++minute;
    }
    if (minute > 59){
        minute = 0;
        ++hour;
    }
    if (hour > 23){
        hour = 0;
    }
    
    clearAllClock();
    hour = hour % 12;
    int minuteLED = minute / 5;
    int secondLED = second / 5;

    setNumberOnClock(hour);
    setNumberOnClock(minuteLED);
    setNumberOnClock(secondLED);
}

#endif /* SRC_EX6_10_H_ */

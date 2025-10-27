/*
 * tasks.c
 *
 *  Created on: Oct 24, 2025
 *      Author: ADMIN
 */

#include "tasks.h"
#include "main.h"
#include "sch.h"

void Task1(void) {
    HAL_GPIO_TogglePin(led0_GPIO_Port, led0_Pin);
}

void Task2(void) {
    HAL_GPIO_TogglePin(led1_GPIO_Port, led1_Pin);
}

void Task3(void) {
    HAL_GPIO_TogglePin(led2_GPIO_Port, led2_Pin);
}

void Task4(void) {
    HAL_GPIO_TogglePin(led3_GPIO_Port, led3_Pin);
}

void Task5(void) {
    HAL_GPIO_TogglePin(led4_GPIO_Port, led4_Pin);
}

void Timer_10ms(void) {
    HAL_GPIO_TogglePin(debug_GPIO_Port, debug_Pin);
}

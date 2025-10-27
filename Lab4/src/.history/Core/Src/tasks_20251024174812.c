/*
 * tasks.c
 *
 *  Created on: Oct 24, 2025
 *      Author: ADMIN
 */

#include "tasks.h"
#include "main.h"
#include "sch.h"

// Tác vụ chạy mỗi 500ms
void Task_500ms(void) {
    HAL_GPIO_TogglePin(led0_GPIO_Port, led0_Pin);
}

// Tác vụ chạy mỗi 1000ms (1 giây)
void Task_1000ms(void) {
    HAL_GPIO_TogglePin(led1_GPIO_Port, led1_Pin);
}

// Tác vụ chạy mỗi 1500ms (1.5 giây)
void Task_1500ms(void) {
    HAL_GPIO_TogglePin(led2_GPIO_Port, led2_Pin);
}

// Tác vụ chạy mỗi 2000ms (2 giây)
void Task_2000ms(void) {
    HAL_GPIO_TogglePin(led3_GPIO_Port, led3_Pin);
}

// Tác vụ chạy mỗi 2500ms (2.5 giây)
void Task_2500ms(void) {
    HAL_GPIO_TogglePin(led4_GPIO_Port, led4_Pin);
}

// Tác vụ chạy mỗi 10ms - toggle debug pin
void Task_10ms(void) {
    HAL_GPIO_TogglePin(debug_GPIO_Port, debug_Pin);
}
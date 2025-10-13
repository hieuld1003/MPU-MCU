/*
 * timer.c
 *
 *  Created on: Oct 2, 2025
 *      Author: ADMIN
 */

#include "main.h"
#include "input_reading.h"
#include "led_display.h"

// Software timer variables
int timer_counter = 0;
int timer_flag = 0;
int timer_display_counter = 0;

void setTimer(int duration) {
    timer_counter = duration;
    timer_flag = 0;
}

void timerRun(void) {
    if (timer_counter > 0) {
        timer_counter--;
        if (timer_counter <= 0) {
            timer_flag = 1;
        }
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM2) {
        // Process button inputs every 10ms
        button_reading();
        
        // Run software timer
        timerRun();
        
        // Update 7-segment displays (multiplexing)
        timer_display_counter++;
        if (timer_display_counter >= 25) {  // Update every 250ms
            timer_display_counter = 0;
            update_seven_segment_led();
        }
    }
}
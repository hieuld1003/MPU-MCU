/*
 * timer.c
 *
 *  Created on: Oct 2, 2025
 *      Author: ADMIN
 */

#include "main.h"
#include "input_reading.h"
#include "led_display.h"

// Software timer definitions
#define NUM_TIMERS 3

int timer_counter[NUM_TIMERS];
int timer_flag[NUM_TIMERS];

// Timer indexes
#define DISPLAY_TIMER 0    // For 7-segment scanning
#define BLINK_TIMER 1      // For LED blinking
#define TRAFFIC_TIMER 2    // For traffic light countdown

void setTimer(int timer_index, int duration) {
    if (timer_index >= 0 && timer_index < NUM_TIMERS) {
        timer_counter[timer_index] = duration;
        timer_flag[timer_index] = 0;
    }
}

void runTimers() {
    for (int i = 0; i < NUM_TIMERS; i++) {
        if (timer_counter[i] > 0) {
            timer_counter[i]--;
            if (timer_counter[i] <= 0) {
                timer_flag[i] = 1;
            }
        }
    }
}

void timerInit() {
    // Initialize all timers
    for (int i = 0; i < NUM_TIMERS; i++) {
        timer_counter[i] = 0;
        timer_flag[i] = 0;
    }
    
    // Set initial timers
    setTimer(DISPLAY_TIMER, 25);   // 25*10ms = 250ms for display update
    setTimer(BLINK_TIMER, 50);     // 50*10ms = 500ms for 2Hz blinking
    setTimer(TRAFFIC_TIMER, 100);  // 100*10ms = 1s for traffic countdown
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM2) {
        // Read button states
        button_reading();
        
        // Run software timers
        runTimers();
        
        // Handle display updating at regular intervals
        if (timer_flag[DISPLAY_TIMER]) {
            update7SEG();
            setTimer(DISPLAY_TIMER, 25); // Reset timer
        }
        
        // Handle traffic light logic and blinking based on mode
        if (timer_flag[BLINK_TIMER]) {
            toggleModeLEDs();
            setTimer(BLINK_TIMER, 50); // Reset timer for 2Hz
        }
        
        // Handle traffic light countdown in normal mode
        if (timer_flag[TRAFFIC_TIMER] && getMode() == 1) {
            // Update traffic light countdown logic
            // Implementation depends on your traffic light FSM
            setTimer(TRAFFIC_TIMER, 100); // Reset timer
        }
    }
}
/*
 * timer.c
 *
 *  Created on: Oct 2, 2025
 *      Author: ADMIN
 */

#include "timer.h"
#include "main.h"
#include "input_reading.h"

// Timer variables
int timerForBlink = 0;
int timerForMode1 = 0;
int timerForAutoInc = 0;

// Traffic light durations (in seconds)
int redDuration = 5;
int amberDuration = 2;
int greenDuration = 3;

// Traffic light counters
int horizontalCounter = 0;
int verticalCounter = 0;

void initTimers(void) {
    timerForBlink = 0;
    timerForMode1 = 0;
    timerForAutoInc = 0;

    horizontalCounter = redDuration;
    verticalCounter = greenDuration;
}

void setTimer(int *timer, int duration) {
    *timer = duration / TICK_TIME;
}

int timerFlag(int *timer) {
    if (*timer <= 0) return 1;
    return 0;
}

void timerRun(void) {
    if (timerForBlink > 0) timerForBlink--;
    if (timerForMode1 > 0) timerForMode1--;
    if (timerForAutoInc > 0) timerForAutoInc--;
}

void updateTimers(void) {
    timerRun();
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if(htim->Instance == TIM2) {
        button_reading();
        updateTimers();
    }
}
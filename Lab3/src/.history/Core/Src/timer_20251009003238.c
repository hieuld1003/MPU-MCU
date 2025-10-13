/*
 * timer.c
 *
 *  Created on: Oct 2, 2025
 *      Author: ADMIN
 */

#include "main.h"
#include "input_reading.h"
#include "led_display.h"
#include "timer.h"

int timer1_counter = 0;
int timer1_flag = 0;

int timer2_counter = 0;
int timer2_flag = 0;

extern int led_index;

void setTimer1(int duration){
    timer1_counter = duration;
    timer1_flag = 0;
}

void setTimer2(int duration){
    timer2_counter = duration;
    timer2_flag = 0;
}

void timer_run(){
    if(timer1_counter > 0){
        timer1_counter--;
        if(timer1_counter <= 0){
            timer1_flag = 1;
        }
    }

    if(timer2_counter > 0){
        timer2_counter--;
        if(timer2_counter <= 0){
            timer2_flag = 1;
        }
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
  if(htim->Instance == TIM2){
    button_reading();
    timer_run();

    // Quét LED 7 đoạn
    if(timer2_flag == 1){
        update7SEG(led_index++);
        if(led_index >= 4) led_index = 0;
        setTimer2(250); // Quét 4 LED với tần số 1Hz (1000ms / 4 = 250ms)
    }
  }
}
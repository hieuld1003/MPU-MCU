/*
 * timer.c
 *
 *  Created on: Oct 2, 2025
 *      Author: ADMIN
 */

#include "main.h"
#include "input_reading.h"
#include "timer.h"

int timer_traffic_light_counter = 0;
int timer_traffic_light_flag = 0;

int timer_7seg_scan_counter = 0;
int timer_7seg_scan_flag = 0;

void setTimer_TrafficLight(int duration){
    timer_traffic_light_counter = duration / 10; // Convert ms to 10ms ticks
    timer_traffic_light_flag = 0;
}

void setTimer_7seg_scan(int duration){
    timer_7seg_scan_counter = duration / 10; // Convert ms to 10ms ticks
    timer_7seg_scan_flag = 0;
}

void timer_run(){
    if(timer_traffic_light_counter > 0){
        timer_traffic_light_counter--;
        if(timer_traffic_light_counter <= 0){
            timer_traffic_light_flag = 1;
        }
    }

    if(timer_7seg_scan_counter > 0){
        timer_7seg_scan_counter--;
        if(timer_7seg_scan_counter <= 0){
            timer_7seg_scan_flag = 1;
        }
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
  if(htim->Instance == TIM2){
    button_reading();
    timer_run();
  }
}
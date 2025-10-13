/*
 * input_processing.c
 *
 *  Created on: Oct 2, 2025
 *      Author: ADMIN
 */

#include "input_processing.h"
#include "led_display.h"
#include "timer.h"
#include "input_reading.h"
#include <stdio.h>

// Default time durations (in seconds)
int time_red = 5;
int time_yellow = 2;
int time_green = 3;

static int mode = NORMAL_MODE;
static int status = RED_H_GREEN_V;

static int counter_h = 0;
static int counter_v = 0;

void fsm_for_input_processing(void){
    static int is_initialized = 0;
    if (!is_initialized) {
        // One-time initialization
        status = RED_H_GREEN_V;
        counter_h = time_red;
        counter_v = time_green;
        setTimer_TrafficLight(1000); // Start 1-second tick
        setTimer_7seg_scan(250);     // Start 7-seg scan
        is_initialized = 1;
    }

    // Run 7-segment display scanning continuously
    scan7SEG();

    switch(mode){
        case NORMAL_MODE:
            if(timer_traffic_light_flag == 1){
                setTimer_TrafficLight(1000); // Reset 1-second timer
                counter_h--;
                counter_v--;

                if(counter_v <= 0){
                    if(status == RED_H_GREEN_V){
                        status = RED_H_YELLOW_V;
                        counter_v = time_yellow;
                    }
                }

                if(counter_h <= 0){
                     if(status == GREEN_H_RED_V){
                        status = YELLOW_H_RED_V;
                        counter_h = time_yellow;
                    }
                }

                if(counter_v <= 0 && status == RED_H_YELLOW_V){
                    status = GREEN_H_RED_V;
                    counter_h = time_green;
                    counter_v = time_red;
                }

                if(counter_h <= 0 && status == YELLOW_H_RED_V){
                    status = RED_H_GREEN_V;
                    counter_h = time_red;
                    counter_v = time_green;
                }
            }
            updateLedBuffer(counter_h, counter_v);
            setTrafficLight(status);
            break;

        case MODIFY_RED_MODE:
            // Logic for Mode 2 will be implemented here
            break;
        case MODIFY_YELLOW_MODE:
            // Logic for Mode 3 will be implemented here
            break;
        case MODIFY_GREEN_MODE:
            // Logic for Mode 4 will be implemented here
            break;
    }
}
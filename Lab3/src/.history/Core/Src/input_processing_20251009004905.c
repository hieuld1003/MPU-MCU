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


// Thời gian mặc định (giây)
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
        // Khởi tạo một lần duy nhất
        status = RED_H_GREEN_V;
        counter_h = time_red;
        counter_v = time_green;
        setTimer_TrafficLight(1000); // Bắt đầu tick 1 giây
        setTimer_7seg_scan(250);     // Bắt đầu quét LED 7-seg
        is_initialized = 1;
    }

    // Luôn chạy quét LED 7 đoạn
    scan7SEG();

    switch(mode){
        case NORMAL_MODE:
            if(timer_traffic_light_flag == 1){
                setTimer_TrafficLight(1000); // Đặt lại timer 1 giây
                counter_h--;
                counter_v--;

                switch(status) {
                    case RED_H_GREEN_V:
                        if (counter_v <= 0) {
                            status = RED_H_YELLOW_V;
                            counter_v = time_yellow;
                        }
                        break;
                    case RED_H_YELLOW_V:
                        if (counter_v <= 0) {
                            status = GREEN_H_RED_V;
                            counter_h = time_green;
                            counter_v = time_red;
                        }
                        break;
                    case GREEN_H_RED_V:
                        if (counter_h <= 0) {
                            status = YELLOW_H_RED_V;
                            counter_h = time_yellow;
                        }
                        break;
                    case YELLOW_H_RED_V:
                        if (counter_h <= 0) {
                            status = RED_H_GREEN_V;
                            counter_h = time_red;
                            counter_v = time_green;
                        }
                        break;
                }
            }
            // Cập nhật buffer cho LED 7 đoạn
            if (status == GREEN_H_RED_V || status == YELLOW_H_RED_V) {
                updateLedBuffer(counter_h, counter_v);
            } else {
                updateLedBuffer(counter_v, counter_h);
            }
            setTrafficLight(status);
            break;

        case MODIFY_RED_MODE:
            // Logic cho Mode 2
            break;
        case MODIFY_YELLOW_MODE:
            // Logic cho Mode 3
            break;
        case MODIFY_GREEN_MODE:
            // Logic cho Mode 4
            break;
    }
}
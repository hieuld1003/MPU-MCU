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

// Trạng thái và bộ đếm riêng cho từng chiều
static int status_h = GREEN_H_RED_V; // Bắt đầu với chiều ngang (H) đèn xanh
static int status_v = RED_H_GREEN_V; // Bắt đầu với chiều dọc (V) đèn đỏ

static int counter_h = 0;
static int counter_v = 0;

void fsm_for_input_processing(void){
    static int is_initialized = 0;
    if (!is_initialized) {
        // Khởi tạo một lần duy nhất
        status_h = GREEN_H_RED_V;
        status_v = RED_H_GREEN_V; // Thực ra trạng thái đèn V phụ thuộc vào H
        counter_h = time_green;
        counter_v = time_red;
        setTimer_TrafficLight(1000); // Bắt đầu tick 1 giây
        setTimer_7seg_scan(250);     // Bắt đầu quét LED 7-seg
        is_initialized = 1;
    }

    // Luôn chạy quét LED 7 đoạn
    scan7SEG();

    switch(mode){
        case NORMAL_MODE:
            // Cập nhật trạng thái đèn và buffer hiển thị
            setTrafficLight(status_h); // Chỉ cần set 1 trạng thái là đủ
            updateLedBuffer(counter_h, counter_v);

            if(timer_traffic_light_flag == 1){
                setTimer_TrafficLight(1000); // Đặt lại timer 1 giây
                counter_h--;
                counter_v--;

                // Máy trạng thái cho chiều ngang (Horizontal)
                switch(status_h) {
                    case GREEN_H_RED_V:
                        if (counter_h <= 0) {
                            status_h = YELLOW_H_RED_V;
                            counter_h = time_yellow;
                        }
                        break;
                    case YELLOW_H_RED_V:
                        if (counter_h <= 0) {
                            status_h = RED_H_GREEN_V;
                            counter_h = time_red;
                            counter_v = time_green; // Đồng bộ lại counter_v
                        }
                        break;
                    case RED_H_GREEN_V:
                         if (counter_h <= 0) {
                            status_h = RED_H_YELLOW_V;
                            // counter_h không đổi, vẫn là 1 phần của đèn đỏ
                         }
                         break;
                    case RED_H_YELLOW_V:
                        if(counter_v <= 0){ // Chuyển trạng thái dựa vào đèn vàng của chiều V
                            status_h = GREEN_H_RED_V;
                            counter_h = time_green;
                            counter_v = time_red; // Đồng bộ lại counter_v
                        }
                        break;
                }
            }
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
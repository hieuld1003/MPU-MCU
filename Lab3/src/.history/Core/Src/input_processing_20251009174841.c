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
static int temp_duration = 0;

// Biến trạng thái và biến đếm cho 2 FSM độc lập
static int status_h = H_RED;
static int status_v = V_GREEN;
static int counter_h = 0;
static int counter_v = 0;

// void reset_to_normal_mode(){
//     mode = NORMAL_MODE;
//     // Khởi tạo lại trạng thái đèn giao thông
//     status_h = H_RED;
//     counter_h = time_red;
//     set_red_h(1); set_yellow_h(0); set_green_h(0);

//     status_v = V_GREEN;
//     counter_v = time_green;
//     set_red_v(0); set_yellow_v(0); set_green_v(1);

//     setTimer_TrafficLight(1000);
}

void fsm_for_input_processing(void){
    // Luôn chạy quét LED 7 đoạn
    scan7SEG();

    // Xử lý chuyển mode bằng sw0
    if(is_button_pressed(0) == 1){
        mode++;
        if(mode > MODIFY_GREEN_MODE) {
            reset_to_normal_mode();
        } else {
            // Tắt hết đèn giao thông khi vào mode chỉnh sửa
            set_red_h(0); set_yellow_h(0); set_green_h(0);
            set_red_v(0); set_yellow_v(0); set_green_v(0);
            setTimer_Blinking(250); // Bắt đầu timer nhấp nháy 2Hz (500ms/chu kỳ)
            switch(mode) {
                case MODIFY_RED_MODE: temp_duration = time_red; break;
                case MODIFY_YELLOW_MODE: temp_duration = time_yellow; break;
                case MODIFY_GREEN_MODE: temp_duration = time_green; break;
            }
        }
    }

    switch(mode){
        case NORMAL_MODE:
            if(timer_traffic_light_flag == 1){
                setTimer_TrafficLight(1000);
                counter_h--;
                counter_v--;
            }
            // FSM cho đèn chiều ngang (Horizontal)
            switch(status_h) {
                case H_RED: if (counter_h <= 0) { status_h = H_GREEN; counter_h = time_green; set_red_h(0); set_green_h(1); } break;
                case H_GREEN: if (counter_h <= 0) { status_h = H_YELLOW; counter_h = time_yellow; set_green_h(0); set_yellow_h(1); } break;
                case H_YELLOW: if (counter_h <= 0) { status_h = H_RED; counter_h = time_red; set_yellow_h(0); set_red_h(1); } break;
            }
            // FSM cho đèn chiều dọc (Vertical)
            switch(status_v) {
                case V_RED: if (counter_v <= 0) { status_v = V_GREEN; counter_v = time_green; set_red_v(0); set_green_v(1); } break;
                case V_GREEN: if (counter_v <= 0) { status_v = V_YELLOW; counter_v = time_yellow; set_green_v(0); set_yellow_v(1); } break;
                case V_YELLOW: if (counter_v <= 0) { status_v = V_RED; counter_v = time_red; set_yellow_v(0); set_red_v(1); } break;
            }
            updateLedBuffer(counter_h, counter_v);
            break;

        case MODIFY_RED_MODE:
        case MODIFY_YELLOW_MODE:
        case MODIFY_GREEN_MODE:
            // Xử lý nhấp nháy đèn
            if(timer_blinking_flag == 1){
                setTimer_Blinking(250);
                if(mode == MODIFY_RED_MODE) { set_red_h(-1); set_red_v(-1); } // Dùng -1 để toggle
                if(mode == MODIFY_YELLOW_MODE) { set_yellow_h(-1); set_yellow_v(-1); }
                if(mode == MODIFY_GREEN_MODE) { set_green_h(-1); set_green_v(-1); }
            }
            // Xử lý tăng giá trị bằng sw1
            if(is_button_pressed(1) == 1){
                temp_duration++;
                if(temp_duration > 99) temp_duration = 1;
            }
            // Xử lý lưu giá trị bằng sw2
            if(is_button_pressed(2) == 1){
                if(mode == MODIFY_RED_MODE) time_red = temp_duration;
                if(mode == MODIFY_YELLOW_MODE) time_yellow = temp_duration;
                if(mode == MODIFY_GREEN_MODE) time_green = temp_duration;
                reset_to_normal_mode();
            }
            updateLedBuffer(temp_duration, mode);
            break;
    }
}
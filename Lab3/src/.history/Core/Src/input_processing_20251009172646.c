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

// Biến trạng thái và biến đếm cho 2 FSM độc lập
static int status_h = H_RED;
static int status_v = V_GREEN;
static int counter_h = 0;
static int counter_v = 0;

void fsm_for_input_processing(void){
    static int is_initialized = 0;
    if (!is_initialized) {
        // Khởi tạo một lần duy nhất
        status_h = H_RED;
        counter_h = time_red;
        set_red_h(1); set_yellow_h(0); set_green_h(0);

        status_v = V_GREEN;
        counter_v = time_green;
        set_red_v(0); set_yellow_v(0); set_green_v(1);

        updateLedBuffer(counter_h, counter_v); // Cập nhật hiển thị ban đầu

        setTimer_TrafficLight(1000); // Bắt đầu tick 1 giây
        setTimer_7seg_scan(500);       // Bắt đầu quét LED 7-seg với tần số cao (5ms)
        is_initialized = 1;
    }

    // Luôn chạy quét LED 7 đoạn để mắt người thấy đèn sáng liên tục
    scan7SEG();

    // Chỉ xử lý logic và cập nhật khi có tick 1 giây
    if(timer_traffic_light_flag == 1){
        setTimer_TrafficLight(1000); // Đặt lại timer cho giây tiếp theo

        // Giảm bộ đếm
        counter_h--;
        counter_v--;

        // Cập nhật buffer cho LED 7-seg ngay sau khi giảm bộ đếm
        updateLedBuffer(counter_h, counter_v);

        // FSM cho đèn chiều ngang (Horizontal)
        switch(status_h) {
            case H_RED:
                if (counter_h <= 0) {
                    status_h = H_GREEN;
                    counter_h = time_green;
                    set_red_h(0); set_green_h(1);
                    updateLedBuffer(counter_h, counter_v); // Cập nhật lại buffer ngay khi đổi trạng thái
                }
                break;
            case H_GREEN:
                if (counter_h <= 0) {
                    status_h = H_YELLOW;
                    counter_h = time_yellow;
                    set_green_h(0); set_yellow_h(1);
                    updateLedBuffer(counter_h, counter_v);
                }
                break;
            case H_YELLOW:
                if (counter_h <= 0) {
                    status_h = H_RED;
                    counter_h = time_red;
                    set_yellow_h(0); set_red_h(1);
                    updateLedBuffer(counter_h, counter_v);
                }
                break;
        }

        // FSM cho đèn chiều dọc (Vertical)
        switch(status_v) {
            case V_RED:
                if (counter_v <= 0) {
                    status_v = V_GREEN;
                    counter_v = time_green;
                    set_red_v(0); set_green_v(1);
                    updateLedBuffer(counter_h, counter_v);
                }
                break;
            case V_GREEN:
                if (counter_v <= 0) {
                    status_v = V_YELLOW;
                    counter_v = time_yellow;
                    set_green_v(0); set_yellow_v(1);
                    updateLedBuffer(counter_h, counter_v);
                }
                break;
            case V_YELLOW:
                if (counter_v <= 0) {
                    status_v = V_RED;
                    counter_v = time_red;
                    set_yellow_v(0); set_red_v(1);
                    updateLedBuffer(counter_h, counter_v);
                }
                break;
        }
    }
}
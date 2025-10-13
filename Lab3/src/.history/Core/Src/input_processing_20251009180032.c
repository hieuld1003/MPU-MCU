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

// Biến trạng thái và biến đếm cho 2 FSM độc lập
static int status_h = H_RED;
static int status_v = V_GREEN;
static int counter_h = 0;
static int counter_v = 0;

// Hàm tắt tất cả đèn giao thông
void turnOffAllLeds() {
    set_red_h(0); set_yellow_h(0); set_green_h(0);
    set_red_v(0); set_yellow_v(0); set_green_v(0);
}

void fsm_for_input_processing(void){
    static int is_normal_initialized = 0;

    // Luôn chạy quét LED 7 đoạn
    scan7SEG();

    // Xử lý nhấn nút chuyển mode
    if (is_button_pressed(0)) {
        mode++;
        if (mode > MODIFY_GREEN_MODE) {
            mode = NORMAL_MODE;
        }
        is_normal_initialized = 0; // Reset cờ khởi tạo khi chuyển mode
        turnOffAllLeds(); // Tắt hết đèn khi chuyển mode
    }

    switch(mode) {
        case NORMAL_MODE:
            if (!is_normal_initialized) {
                // Khởi tạo lại trạng thái đèn giao thông
                status_h = H_RED;
                counter_h = time_red;
                set_red_h(1);

                status_v = V_GREEN;
                counter_v = time_green;
                set_green_v(1);

                setTimer_TrafficLight(1000);
                is_normal_initialized = 1;
            }

            if(timer_traffic_light_flag == 1){
                setTimer_TrafficLight(1000);
                counter_h--;
                counter_v--;
            }
            updateLedBuffer(counter_h, counter_v);

            // FSM cho đèn chiều ngang (Horizontal)
            switch(status_h) {
                case H_RED:
                    if (counter_h <= 0) { status_h = H_GREEN; counter_h = time_green; set_red_h(0); set_green_h(1); }
                    break;
                case H_GREEN:
                    if (counter_h <= 0) { status_h = H_YELLOW; counter_h = time_yellow; set_green_h(0); set_yellow_h(1); }
                    break;
                case H_YELLOW:
                    if (counter_h <= 0) { status_h = H_RED; counter_h = time_red; set_yellow_h(0); set_red_h(1); }
                    break;
            }

            // FSM cho đèn chiều dọc (Vertical)
            switch(status_v) {
                case V_RED:
                    if (counter_v <= 0) { status_v = V_GREEN; counter_v = time_green; set_red_v(0); set_green_v(1); }
                    break;
                case V_GREEN:
                    if (counter_v <= 0) { status_v = V_YELLOW; counter_v = time_yellow; set_green_v(0); set_yellow_v(1); }
                    break;
                case V_YELLOW:
                    if (counter_v <= 0) { status_v = V_RED; counter_v = time_red; set_yellow_v(0); set_red_v(1); }
                    break;
            }
            break;

        case MODIFY_RED_MODE:
            updateLedBuffer(2, time_red); // Hiển thị mode 02 và giá trị time_red
            if (timer_blink_flag == 1) {
                setTimer_Blink(250); // 2Hz -> 500ms period -> 250ms toggle
                HAL_GPIO_TogglePin(red_h_GPIO_Port, red_h_Pin);
                HAL_GPIO_TogglePin(red_v_GPIO_Port, red_v_Pin);
            }
            break;

        case MODIFY_YELLOW_MODE:
            updateLedBuffer(3, time_yellow); // Hiển thị mode 03 và giá trị time_yellow
            if (timer_blink_flag == 1) {
                setTimer_Blink(250);
                HAL_GPIO_TogglePin(yellow_h_GPIO_Port, yellow_h_Pin);
                HAL_GPIO_TogglePin(yellow_v_GPIO_Port, yellow_v_Pin);
            }
            break;

        case MODIFY_GREEN_MODE:
            updateLedBuffer(4, time_green); // Hiển thị mode 04 và giá trị time_green
            if (timer_blink_flag == 1) {
                setTimer_Blink(250);
                HAL_GPIO_TogglePin(green_h_GPIO_Port, green_h_Pin);
                HAL_GPIO_TogglePin(green_v_GPIO_Port, green_v_Pin);
            }
            break;
    }
}
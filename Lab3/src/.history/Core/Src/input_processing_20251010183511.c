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

// Biến tạm để lưu giá trị đang chỉnh sửa
static int temp_value = 0;

static int mode = NORMAL_MODE;
static int prev_mode = NORMAL_MODE; 

// Biến trạng thái và biến đếm cho 2 FSM độc lập
static int status_h = H_RED;
static int status_v = V_GREEN;
static int counter_h = 0;
static int counter_v = 0;

// Biến theo dõi trạng thái nút nhấn
static int button0_flag = 0;
static int button1_flag = 0;
static int button2_flag = 0;

// Thêm biến đếm cho chức năng tự động tăng giá trị
static int auto_increase_counter = 0;

// Biến lưu trạng thái nhấp nháy
static int blink_status = 0;

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

        button0_flag = 1;
        button1_flag = 1;
        button2_flag = 1;

        setTimer_TrafficLight(1000);
        setTimer_7seg_scan(10);
        setTimer_Blink(250);
        is_initialized = 1;
    }

    // Luôn chạy quét LED 7 đoạn
    scan7SEG();
    
    // Xử lý nút nhấn sw0 để chuyển mode
    if (is_button_pressed(0) && !button0_flag) {
        button0_flag = 1;
        prev_mode = mode; // Lưu mode hiện tại trước khi thay đổi
        mode = (mode % 4) + 1; // Chuyển đổi giữa mode 1-4
        
        // Thiết lập giá trị tạm khi vào mode chỉnh sửa
        switch(mode) {
            case MODIFY_RED_MODE:
                temp_value = time_red;
                break;
            case MODIFY_YELLOW_MODE:
                temp_value = time_yellow;
                break;
            case MODIFY_GREEN_MODE:
                temp_value = time_green;
                break;
        }
        
        // Khi chuyển từ mode 4 về mode 1, reset lại trạng thái
        if (prev_mode == MODIFY_GREEN_MODE && mode == NORMAL_MODE) {
            status_h = H_RED;
            counter_h = time_red;
            set_red_h(1); set_yellow_h(0); set_green_h(0);

            status_v = V_GREEN;
            counter_v = time_green;
            set_red_v(0); set_yellow_v(0); set_green_v(1);
            
            setTimer_TrafficLight(1000); // Đặt lại timer 1 giây
        }
    } else if (!is_button_pressed(0)) {
        button0_flag = 0;
    }
    
    // Xử lý nút nhấn sw1 để tăng giá trị
    if (is_button_pressed(1)) {
        if (!button1_flag) {
            button1_flag = 1;
            
            // Chỉ tăng giá trị khi ở mode chỉnh sửa
            if (mode != NORMAL_MODE) {
                temp_value++;
                if (temp_value > 99) temp_value = 1; // Giới hạn 1-99
            }
        }
        
        // Xử lý tự động tăng giá trị khi giữ nút
        if (is_button_pressed_1s(1)) {
            auto_increase_counter++;
            // Tăng giá trị mỗi 100 chu kỳ (tương đương 1 giây với timer 10ms)
            if (auto_increase_counter >= 4000) {
                auto_increase_counter = 0;
                if (mode != NORMAL_MODE) {
                    temp_value++;
                    if (temp_value > 99) temp_value = 1; // Giới hạn 1-99
                }
            }
        }
    } else {
        button1_flag = 0;
        auto_increase_counter = 0; // Reset bộ đếm khi thả nút
    }
    
    // Xử lý nút nhấn sw2 để xác nhận giá trị
    if (is_button_pressed(2) && !button2_flag) {
        button2_flag = 1;
        
        // Chỉ xác nhận giá trị khi ở mode chỉnh sửa
        if (mode != NORMAL_MODE) {
            // Lưu giá trị tùy theo mode hiện tại
            switch(mode) {
                case MODIFY_RED_MODE:
                    time_red = temp_value;
                    // Đảm bảo time_red = time_green + time_yellow
                    // Ưu tiên điều chỉnh time_green
                    if (time_green + time_yellow != time_red) {
                        int diff = time_red - time_yellow; // Phần còn lại sau khi trừ time_yellow
                        
                        if (diff > 1) {
                            time_green = diff; // Đảm bảo time_green là giá trị dương
                        } else {
                            time_green = 1;
                            time_yellow = time_red - time_green; // Điều chỉnh time_yellow
                        }
                    }
                    break;
                    
                case MODIFY_YELLOW_MODE:
                    time_yellow = temp_value;
                    // Đảm bảo time_red = time_green + time_yellow
                    // Ưu tiên điều chỉnh time_green
                    if (time_green + time_yellow != time_red) {
                        time_green = time_red - time_yellow;
                        if (time_green < 1) {
                            time_green = 1;
                            time_red = time_green + time_yellow; // Điều chỉnh lại time_red nếu cần
                        }
                    }
                    break;
                    
                case MODIFY_GREEN_MODE:
                    time_green = temp_value;
                    // Đảm bảo time_red = time_green + time_yellow
                    // Ưu tiên điều chỉnh time_yellow
                    if (time_green + time_yellow != time_red) {
                        time_yellow = time_red - time_green;
                        if (time_yellow < 1) {
                            time_yellow = 1;
                            time_red = time_green + time_yellow; // Điều chỉnh lại time_red nếu cần
                        }
                    }
                    break;
            }
        }
    } else if (!is_button_pressed(2)) {
        button2_flag = 0;
    }
    
    // Xử lý các mode khác nhau
    switch(mode) {
        case NORMAL_MODE:
            updateLedBuffer(counter_h, counter_v);
        
            if(timer_traffic_light_flag == 1){
                setTimer_TrafficLight(500); // Đặt lại timer 1 giây
                counter_h--;
                counter_v--;
            }

            // FSM cho đèn chiều ngang (Horizontal)
            switch(status_h) {
                case H_RED:
                    if (counter_h <= 0) {
                        status_h = H_GREEN;
                        counter_h = time_green;
                        set_red_h(0); set_green_h(1);
                    }
                    break;
                case H_GREEN:
                    if (counter_h <= 0) {
                        status_h = H_YELLOW;
                        counter_h = time_yellow;
                        set_green_h(0); set_yellow_h(1);
                    }
                    break;
                case H_YELLOW:
                    if (counter_h <= 0) {
                        status_h = H_RED;
                        counter_h = time_red;
                        set_yellow_h(0); set_red_h(1);
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
                    }
                    break;
                case V_GREEN:
                    if (counter_v <= 0) {
                        status_v = V_YELLOW;
                        counter_v = time_yellow;
                        set_green_v(0); set_yellow_v(1);
                    }
                    break;
                case V_YELLOW:
                    if (counter_v <= 0) {
                        status_v = V_RED;
                        counter_v = time_red;
                        set_yellow_v(0); set_red_v(1);
                    }
                    break;
            }
            break;
            
        case MODIFY_RED_MODE:
            // Hiển thị mode 2 trên 2 đèn LED 7-đoạn phải
            led_buffer[2] = 0;
            led_buffer[3] = 2;
            
            // Hiển thị giá trị tạm thời đang chỉnh sửa
            led_buffer[0] = temp_value / 10;
            led_buffer[1] = temp_value % 10;
            
            // Nhấp nháy đèn đỏ ở tần số 2Hz
            if (timer_blink_flag == 1) {
                setTimer_Blink(250); // 250ms = 2Hz
                blink_status = 1 - blink_status;
                
                // Điều khiển đèn nhấp nháy
                set_red_h(blink_status);
                set_red_v(blink_status);
                set_yellow_h(0);
                set_yellow_v(0);
                set_green_h(0);
                set_green_v(0);
            }
            break;
            
        case MODIFY_YELLOW_MODE:
            // Hiển thị mode 3 trên 2 đèn LED 7-đoạn phải
            led_buffer[2] = 0;
            led_buffer[3] = 3;
            
            // Hiển thị giá trị tạm thời đang chỉnh sửa
            led_buffer[0] = temp_value / 10;
            led_buffer[1] = temp_value % 10;
            
            // Nhấp nháy đèn vàng ở tần số 2Hz
            if (timer_blink_flag == 1) {
                setTimer_Blink(250); // 250ms = 2Hz
                blink_status = 1 - blink_status;
                
                // Điều khiển đèn nhấp nháy
                set_red_h(0);
                set_red_v(0);
                set_yellow_h(blink_status);
                set_yellow_v(blink_status);
                set_green_h(0);
                set_green_v(0);
            }
            break;
            
        case MODIFY_GREEN_MODE:
            // Hiển thị mode 4 trên 2 đèn LED 7-đoạn phải
            led_buffer[2] = 0;
            led_buffer[3] = 4;
            
            // Hiển thị giá trị tạm thời đang chỉnh sửa
            led_buffer[0] = temp_value / 10;
            led_buffer[1] = temp_value % 10;
            
            // Nhấp nháy đèn xanh ở tần số 2Hz
            if (timer_blink_flag == 1) {
                setTimer_Blink(250); // 250ms = 2Hz
                blink_status = 1 - blink_status;
                
                // Điều khiển đèn nhấp nháy
                set_red_h(0);
                set_red_v(0);
                set_yellow_h(0);
                set_yellow_v(0);
                set_green_h(blink_status);
                set_green_v(blink_status);
            }
            break;
    }
}
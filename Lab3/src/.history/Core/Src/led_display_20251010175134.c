/*
 * led_display.c
 *
 *  Created on: Oct 2, 2025
 *      Author: ADMIN
 */

#include "led_display.h"
#include "timer.h"
#include "input_processing.h"

int led_buffer[4] = {0, 0, 0, 0};
// 0: Hàng chục ngang, 1: Hàng đơn vị ngang
// 2: Hàng chục dọc,   3: Hàng đơn vị dọc

// Mã cho LED 7 đoạn (Common Anode), 0=ON, 1=OFF
static uint8_t seven_seg_code[10] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90};

// Hàm hiển thị cho 2 LED 7-seg chiều ngang
void display7SEG_H(int num) {
    uint8_t code = seven_seg_code[num];
    HAL_GPIO_WritePin(a_h_GPIO_Port, a_h_Pin, (code >> 0) & 0x01);
    HAL_GPIO_WritePin(b_h_GPIO_Port, b_h_Pin, (code >> 1) & 0x01);
    HAL_GPIO_WritePin(c_h_GPIO_Port, c_h_Pin, (code >> 2) & 0x01);
    HAL_GPIO_WritePin(d_h_GPIO_Port, d_h_Pin, (code >> 3) & 0x01);
    HAL_GPIO_WritePin(e_h_GPIO_Port, e_h_Pin, (code >> 4) & 0x01);
    HAL_GPIO_WritePin(f_h_GPIO_Port, f_h_Pin, (code >> 5) & 0x01);
    HAL_GPIO_WritePin(g_h_GPIO_Port, g_h_Pin, (code >> 6) & 0x01);
}

// Hàm hiển thị cho 2 LED 7-seg chiều dọc
void display7SEG_V(int num) {
    uint8_t code = seven_seg_code[num];
    HAL_GPIO_WritePin(a_v_GPIO_Port, a_v_Pin, (code >> 0) & 0x01);
    HAL_GPIO_WritePin(b_v_GPIO_Port, b_v_Pin, (code >> 1) & 0x01);
    HAL_GPIO_WritePin(c_v_GPIO_Port, c_v_Pin, (code >> 2) & 0x01);
    HAL_GPIO_WritePin(d_v_GPIO_Port, d_v_Pin, (code >> 3) & 0x01);
    HAL_GPIO_WritePin(e_v_GPIO_Port, e_v_Pin, (code >> 4) & 0x01);
    HAL_GPIO_WritePin(f_v_GPIO_Port, f_v_Pin, (code >> 5) & 0x01);
    HAL_GPIO_WritePin(g_v_GPIO_Port, g_v_Pin, (code >> 6) & 0x01);
}

// Biến trạng thái cho việc quét 2 giai đoạn
static int scan_stage = 0;

void scan7SEG() {
    if (timer_7seg_scan_flag == 1) {
        setTimer_7seg_scan(250); // Quét nhanh giữa 2 giai đoạn

        if (scan_stage == 0) {
            // Giai đoạn 1: Bật đèn hàng chục (en0, en2)
            HAL_GPIO_WritePin(en1_GPIO_Port, en1_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(en3_GPIO_Port, en3_Pin, GPIO_PIN_SET);

            display7SEG_H(led_buffer[0]); // Hiển thị số hàng chục ngang
            display7SEG_V(led_buffer[2]); // Hiển thị số hàng chục dọc

            HAL_GPIO_WritePin(en0_GPIO_Port, en0_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(en2_GPIO_Port, en2_Pin, GPIO_PIN_RESET);

            scan_stage = 1; // Chuyển sang giai đoạn tiếp theo
        } else {
            // Giai đoạn 2: Bật đèn hàng đơn vị (en1, en3)
            HAL_GPIO_WritePin(en0_GPIO_Port, en0_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(en2_GPIO_Port, en2_Pin, GPIO_PIN_SET);

            display7SEG_H(led_buffer[1]); // Hiển thị số hàng đơn vị ngang
            display7SEG_V(led_buffer[3]); // Hiển thị số hàng đơn vị dọc

            HAL_GPIO_WritePin(en1_GPIO_Port, en1_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(en3_GPIO_Port, en3_Pin, GPIO_PIN_RESET);

            scan_stage = 0; // Quay lại giai đoạn đầu
        }
    }
}

void updateLedBuffer(int duration_h, int duration_v) {
    led_buffer[0] = duration_h / 10;
    led_buffer[1] = duration_h % 10;
    led_buffer[2] = duration_v / 10;
    led_buffer[3] = duration_v % 10;
}

// Logic ngược: RESET để BẬT, SET để TẮT
void set_red_h(int state) { HAL_GPIO_WritePin(red_h_GPIO_Port, red_h_Pin, !state); }
void set_yellow_h(int state) { HAL_GPIO_WritePin(yellow_h_GPIO_Port, yellow_h_Pin, !state); }
void set_green_h(int state) { HAL_GPIO_WritePin(green_h_GPIO_Port, green_h_Pin, !state); }

void set_red_v(int state) { HAL_GPIO_WritePin(red_v_GPIO_Port, red_v_Pin, !state); }
void set_yellow_v(int state) { HAL_GPIO_WritePin(yellow_v_GPIO_Port, yellow_v_Pin, !state); }
void set_green_v(int state) { HAL_GPIO_WritePin(green_v_GPIO_Port, green_v_Pin, !state); }
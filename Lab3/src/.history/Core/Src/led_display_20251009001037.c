/*
 * led_display.c
 *
 *  Created on: Oct 2, 2025
 *      Author: ADMIN
 */

#include "main.h"
#include "led_display.h"
#include "timer.h"
#include "input_processing.h"

// 7-segment LED segments pattern (0-9)
int led7seg[10] = {
    0x40, // 0: 0b01000000
    0x79, // 1: 0b01111001
    0x24, // 2: 0b00100100
    0x30, // 3: 0b00110000
    0x19, // 4: 0b00011001
    0x12, // 5: 0b00010010
    0x02, // 6: 0b00000010
    0x78, // 7: 0b01111000
    0x00, // 8: 0b00000000
    0x10  // 9: 0b00010000
};

// Traffic light states
int horizontalState = RED;
int verticalState = GREEN;

void displayLED7Seg(int num, int isVertical) {
    if (num < 0 || num > 9) return;
    
    int segments = led7seg[num];
    
    if (isVertical) {
        HAL_GPIO_WritePin(a_v_GPIO_Port, a_v_Pin, (segments & 0x01));
        HAL_GPIO_WritePin(b_v_GPIO_Port, b_v_Pin, (segments & 0x02) >> 1);
        HAL_GPIO_WritePin(c_v_GPIO_Port, c_v_Pin, (segments & 0x04) >> 2);
        HAL_GPIO_WritePin(d_v_GPIO_Port, d_v_Pin, (segments & 0x08) >> 3);
        HAL_GPIO_WritePin(e_v_GPIO_Port, e_v_Pin, (segments & 0x10) >> 4);
        HAL_GPIO_WritePin(f_v_GPIO_Port, f_v_Pin, (segments & 0x20) >> 5);
        HAL_GPIO_WritePin(g_v_GPIO_Port, g_v_Pin, (segments & 0x40) >> 6);
    } else {
        HAL_GPIO_WritePin(a_h_GPIO_Port, a_h_Pin, (segments & 0x01));
        HAL_GPIO_WritePin(b_h_GPIO_Port, b_h_Pin, (segments & 0x02) >> 1);
        HAL_GPIO_WritePin(c_h_GPIO_Port, c_h_Pin, (segments & 0x04) >> 2);
        HAL_GPIO_WritePin(d_h_GPIO_Port, d_h_Pin, (segments & 0x08) >> 3);
        HAL_GPIO_WritePin(e_h_GPIO_Port, e_h_Pin, (segments & 0x10) >> 4);
        HAL_GPIO_WritePin(f_h_GPIO_Port, f_h_Pin, (segments & 0x20) >> 5);
        HAL_GPIO_WritePin(g_h_GPIO_Port, g_h_Pin, (segments & 0x40) >> 6);
    }
}

void updateClockBuffer() {
    displayBuffer[0] = horizontalCounter / 10;
    displayBuffer[1] = horizontalCounter % 10;
    displayBuffer[2] = verticalCounter / 10;
    displayBuffer[3] = verticalCounter % 10;
}

void displayInit() {
    updateClockBuffer();
}

void scan7Seg() {
    // Quét hiển thị LED 7-đoạn
    static int index = 0;
    
    // Tắt tất cả màn hình
    HAL_GPIO_WritePin(en0_GPIO_Port, en0_Pin, 0);
    HAL_GPIO_WritePin(en1_GPIO_Port, en1_Pin, 0);
    HAL_GPIO_WritePin(en2_GPIO_Port, en2_Pin, 0);
    HAL_GPIO_WritePin(en3_GPIO_Port, en3_Pin, 0);
    
    // Hiển thị theo mode hiện tại
    if (status == NORMAL_MODE) {
        if (index == 0 || index == 1) {
            displayLED7Seg(displayBuffer[index], 0); // Hiển thị horizontal
        } else {
            displayLED7Seg(displayBuffer[index], 1); // Hiển thị vertical
        }
    } else {
        // Các mode cấu hình
        if (index == 0) {
            // Hiển thị chữ số hàng chục
            displayLED7Seg(configValue / 10, 0);
        } else if (index == 1) {
            // Hiển thị chữ số hàng đơn vị
            displayLED7Seg(configValue % 10, 0);
        } else if (index == 2) {
            // Hiển thị số 0
            displayLED7Seg(0, 1);
        } else {
            // Hiển thị số mode
            displayLED7Seg(status, 1);
        }
    }
    
    // Bật màn hình tương ứng
    switch (index) {
        case 0:
            HAL_GPIO_WritePin(en0_GPIO_Port, en0_Pin, 1);
            break;
        case 1:
            HAL_GPIO_WritePin(en1_GPIO_Port, en1_Pin, 1);
            break;
        case 2:
            HAL_GPIO_WritePin(en2_GPIO_Port, en2_Pin, 1);
            break;
        case 3:
            HAL_GPIO_WritePin(en3_GPIO_Port, en3_Pin, 1);
            break;
        default:
            break;
    }
    
    // Chuyển đến màn hình tiếp theo
    index = (index + 1) % 4;
}

void setTrafficLight(int horizontal_state, int vertical_state) {
    // Horizontal
    if (horizontal_state == RED) {
        HAL_GPIO_WritePin(red_h_GPIO_Port, red_h_Pin, 0);
        HAL_GPIO_WritePin(yellow_h_GPIO_Port, yellow_h_Pin, 1);
        HAL_GPIO_WritePin(green_h_GPIO_Port, green_h_Pin, 1);
    } else if (horizontal_state == AMBER) {
        HAL_GPIO_WritePin(red_h_GPIO_Port, red_h_Pin, 1);
        HAL_GPIO_WritePin(yellow_h_GPIO_Port, yellow_h_Pin, 0);
        HAL_GPIO_WritePin(green_h_GPIO_Port, green_h_Pin, 1);
    } else if (horizontal_state == GREEN) {
        HAL_GPIO_WritePin(red_h_GPIO_Port, red_h_Pin, 1);
        HAL_GPIO_WritePin(yellow_h_GPIO_Port, yellow_h_Pin, 1);
        HAL_GPIO_WritePin(green_h_GPIO_Port, green_h_Pin, 0);
    }
    
    // Vertical
    if (vertical_state == RED) {
        HAL_GPIO_WritePin(red_v_GPIO_Port, red_v_Pin, 0);
        HAL_GPIO_WritePin(yellow_v_GPIO_Port, yellow_v_Pin, 1);
        HAL_GPIO_WritePin(green_v_GPIO_Port, green_v_Pin, 1);
    } else if (vertical_state == AMBER) {
        HAL_GPIO_WritePin(red_v_GPIO_Port, red_v_Pin, 1);
        HAL_GPIO_WritePin(yellow_v_GPIO_Port, yellow_v_Pin, 0);
        HAL_GPIO_WritePin(green_v_GPIO_Port, green_v_Pin, 1);
    } else if (vertical_state == GREEN) {
        HAL_GPIO_WritePin(red_v_GPIO_Port, red_v_Pin, 1);
        HAL_GPIO_WritePin(yellow_v_GPIO_Port, yellow_v_Pin, 1);
        HAL_GPIO_WritePin(green_v_GPIO_Port, green_v_Pin, 0);
    }
}

void blinkAllRed() {
    static int blink_status = 0;
    if (timer2_flag == 1) {
        setTimer2(500); // 0.5s (2Hz)
        blink_status = !blink_status;
        HAL_GPIO_WritePin(red_h_GPIO_Port, red_h_Pin, blink_status);
        HAL_GPIO_WritePin(red_v_GPIO_Port, red_v_Pin, blink_status);
    }
}

void blinkAllYellow() {
    static int blink_status = 0;
    if (timer2_flag == 1) {
        setTimer2(500); // 0.5s (2Hz)
        blink_status = !blink_status;
        HAL_GPIO_WritePin(yellow_h_GPIO_Port, yellow_h_Pin, blink_status);
        HAL_GPIO_WritePin(yellow_v_GPIO_Port, yellow_v_Pin, blink_status);
    }
}

void blinkAllGreen() {
    static int blink_status = 0;
    if (timer2_flag == 1) {
        setTimer2(500); // 0.5s (2Hz)
        blink_status = !blink_status;
        HAL_GPIO_WritePin(green_h_GPIO_Port, green_h_Pin, blink_status);
        HAL_GPIO_WritePin(green_v_GPIO_Port, green_v_Pin, blink_status);
    }
}

void clearAllLights() {
    HAL_GPIO_WritePin(red_h_GPIO_Port, red_h_Pin, 1);
    HAL_GPIO_WritePin(yellow_h_GPIO_Port, yellow_h_Pin, 1);
    HAL_GPIO_WritePin(green_h_GPIO_Port, green_h_Pin, 1);
    HAL_GPIO_WritePin(red_v_GPIO_Port, red_v_Pin, 1);
    HAL_GPIO_WritePin(yellow_v_GPIO_Port, yellow_v_Pin, 1);
    HAL_GPIO_WritePin(green_v_GPIO_Port, green_v_Pin, 1);
}
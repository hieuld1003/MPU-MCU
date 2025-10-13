/*
 * led_display.c
 *
 *  Created on: Oct 2, 2025
 *      Author: ADMIN
 */

#include "led_display.h"

const int MAX_LED = 4;
int led_buffer[4] = {0, 0, 0, 0};
int led_index = 0;

// Bảng mã cho LED 7 đoạn từ 0 đến 9
uint8_t seven_seg_code[10] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};

void display7SEG_H(int num) {
    uint8_t code = seven_seg_code[num];
    HAL_GPIO_WritePin(a_h_GPIO_Port, a_h_Pin, (code >> 0) & 0x01 ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(b_h_GPIO_Port, b_h_Pin, (code >> 1) & 0x01 ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(c_h_GPIO_Port, c_h_Pin, (code >> 2) & 0x01 ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(d_h_GPIO_Port, d_h_Pin, (code >> 3) & 0x01 ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(e_h_GPIO_Port, e_h_Pin, (code >> 4) & 0x01 ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(f_h_GPIO_Port, f_h_Pin, (code >> 5) & 0x01 ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(g_h_GPIO_Port, g_h_Pin, (code >> 6) & 0x01 ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

void display7SEG_V(int num) {
    uint8_t code = seven_seg_code[num];
    HAL_GPIO_WritePin(a_v_GPIO_Port, a_v_Pin, (code >> 0) & 0x01 ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(b_v_GPIO_Port, b_v_Pin, (code >> 1) & 0x01 ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(c_v_GPIO_Port, c_v_Pin, (code >> 2) & 0x01 ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(d_v_GPIO_Port, d_v_Pin, (code >> 3) & 0x01 ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(e_v_GPIO_Port, e_v_Pin, (code >> 4) & 0x01 ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(f_v_GPIO_Port, f_v_Pin, (code >> 5) & 0x01 ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(g_v_GPIO_Port, g_v_Pin, (code >> 6) & 0x01 ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

void update7SEG(int index) {
    switch (index) {
        case 0: // Display 1st 7SEG for horizontal
            HAL_GPIO_WritePin(en0_GPIO_Port, en0_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(en1_GPIO_Port, en1_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(en2_GPIO_Port, en2_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(en3_GPIO_Port, en3_Pin, GPIO_PIN_SET);
            display7SEG_H(led_buffer[0]);
            break;
        case 1: // Display 2nd 7SEG for horizontal
            HAL_GPIO_WritePin(en0_GPIO_Port, en0_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(en1_GPIO_Port, en1_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(en2_GPIO_Port, en2_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(en3_GPIO_Port, en3_Pin, GPIO_PIN_SET);
            display7SEG_H(led_buffer[1]);
            break;
        case 2: // Display 1st 7SEG for vertical
            HAL_GPIO_WritePin(en0_GPIO_Port, en0_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(en1_GPIO_Port, en1_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(en2_GPIO_Port, en2_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(en3_GPIO_Port, en3_Pin, GPIO_PIN_SET);
            display7SEG_V(led_buffer[2]);
            break;
        case 3: // Display 2nd 7SEG for vertical
            HAL_GPIO_WritePin(en0_GPIO_Port, en0_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(en1_GPIO_Port, en1_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(en2_GPIO_Port, en2_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(en3_GPIO_Port, en3_Pin, GPIO_PIN_RESET);
            display7SEG_V(led_buffer[3]);
            break;
        default:
            break;
    }
}

void updateLedBuffer(int duration1, int duration2) {
    led_buffer[0] = duration1 / 10;
    led_buffer[1] = duration1 % 10;
    led_buffer[2] = duration2 / 10;
    led_buffer[3] = duration2 % 10;
}

void setTrafficLight(int status) {
    switch (status) {
        case 0: // RED_H - GREEN_V
            HAL_GPIO_WritePin(red_h_GPIO_Port, red_h_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(yellow_h_GPIO_Port, yellow_h_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(green_h_GPIO_Port, green_h_Pin, GPIO_PIN_RESET);

            HAL_GPIO_WritePin(red_v_GPIO_Port, red_v_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(yellow_v_GPIO_Port, yellow_v_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(green_v_GPIO_Port, green_v_Pin, GPIO_PIN_SET);
            break;
        case 1: // RED_H - YELLOW_V
            HAL_GPIO_WritePin(red_h_GPIO_Port, red_h_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(yellow_h_GPIO_Port, yellow_h_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(green_h_GPIO_Port, green_h_Pin, GPIO_PIN_RESET);

            HAL_GPIO_WritePin(red_v_GPIO_Port, red_v_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(yellow_v_GPIO_Port, yellow_v_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(green_v_GPIO_Port, green_v_Pin, GPIO_PIN_RESET);
            break;
        case 2: // GREEN_H - RED_V
            HAL_GPIO_WritePin(red_h_GPIO_Port, red_h_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(yellow_h_GPIO_Port, yellow_h_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(green_h_GPIO_Port, green_h_Pin, GPIO_PIN_SET);

            HAL_GPIO_WritePin(red_v_GPIO_Port, red_v_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(yellow_v_GPIO_Port, yellow_v_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(green_v_GPIO_Port, green_v_Pin, GPIO_PIN_RESET);
            break;
        case 3: // YELLOW_H - RED_V
            HAL_GPIO_WritePin(red_h_GPIO_Port, red_h_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(yellow_h_GPIO_Port, yellow_h_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(green_h_GPIO_Port, green_h_Pin, GPIO_PIN_RESET);

            HAL_GPIO_WritePin(red_v_GPIO_Port, red_v_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(yellow_v_GPIO_Port, yellow_v_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(green_v_GPIO_Port, green_v_Pin, GPIO_PIN_RESET);
            break;
        default:
            break;
    }
}
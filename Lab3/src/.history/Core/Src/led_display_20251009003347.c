/*
 * led_display.c
 *
 *  Created on: Oct 2, 2025
 *      Author: ADMIN
 */

#include "led_display.h"
#include "timer.h"

static int led_buffer[4] = {0, 0, 0, 0};
static int led_index = 0;

// Active low for segments
static uint8_t seven_seg_code[10] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};

void display7SEG(int num) {
    uint8_t code = seven_seg_code[num];
    HAL_GPIO_WritePin(a_h_GPIO_Port, a_h_Pin, (code >> 0) & 0x01);
    HAL_GPIO_WritePin(b_h_GPIO_Port, b_h_Pin, (code >> 1) & 0x01);
    HAL_GPIO_WritePin(c_h_GPIO_Port, c_h_Pin, (code >> 2) & 0x01);
    HAL_GPIO_WritePin(d_h_GPIO_Port, d_h_Pin, (code >> 3) & 0x01);
    HAL_GPIO_WritePin(e_h_GPIO_Port, e_h_Pin, (code >> 4) & 0x01);
    HAL_GPIO_WritePin(f_h_GPIO_Port, f_h_Pin, (code >> 5) & 0x01);
    HAL_GPIO_WritePin(g_h_GPIO_Port, g_h_Pin, (code >> 6) & 0x01);
}

void update7SEG(int index) {
    // Active low for enable pins
    HAL_GPIO_WritePin(en0_GPIO_Port, en0_Pin, (index == 0) ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(en1_GPIO_Port, en1_Pin, (index == 1) ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(en2_GPIO_Port, en2_Pin, (index == 2) ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(en3_GPIO_Port, en3_Pin, (index == 3) ? GPIO_PIN_RESET : GPIO_PIN_SET);
    display7SEG(led_buffer[index]);
}

void scan7SEG() {
    if (timer_7seg_scan_flag == 1) {
        setTimer_7seg_scan(250); // 250ms for each 7-seg -> 4Hz scan rate
        update7SEG(led_index);
        led_index = (led_index + 1) % 4;
    }
}

void updateLedBuffer(int duration1, int duration2) {
    led_buffer[0] = duration1 / 10;
    led_buffer[1] = duration1 % 10;
    led_buffer[2] = duration2 / 10;
    led_buffer[3] = duration2 % 10;
}

void setTrafficLight(int status) {
    // Active high for LEDs
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
        default: // Turn off all
            HAL_GPIO_WritePin(GPIOA, red_h_Pin|yellow_h_Pin|green_h_Pin|red_v_Pin|yellow_v_Pin|green_v_Pin, GPIO_PIN_RESET);
            break;
    }
}
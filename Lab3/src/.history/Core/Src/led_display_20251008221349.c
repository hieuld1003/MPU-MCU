/*
 * led_display.c
 *
 *  Created on: Oct 2, 2025
 *      Author: ADMIN
 */

 #include "main.h"
#include "led_display.h"

// Buffer for 7-segment LEDs (4 LEDs: 2 for horizontal traffic light, 2 for vertical traffic light)
int led_buffer[4] = {0, 0, 0, 0};
int led_index = 0;

// 7-segment display patterns for digits 0-9
const int SEG7_DISPLAY[10] = {
    0x01, // 0: 0000 0001 (segments a,b,c,d,e,f on)
    0x4F, // 1: 0100 1111 (segments b,c on)
    0x12, // 2: 0001 0010 (segments a,b,d,e,g on)
    0x06, // 3: 0000 0110 (segments a,b,c,d,g on)
    0x4C, // 4: 0100 1100 (segments b,c,f,g on)
    0x24, // 5: 0010 0100 (segments a,c,d,f,g on)
    0x20, // 6: 0010 0000 (segments a,c,d,e,f,g on)
    0x0F, // 7: 0000 1111 (segments a,b,c on)
    0x00, // 8: 0000 0000 (all segments on)
    0x04  // 9: 0000 0100 (segments a,b,c,d,f,g on)
};

void display7SEG(int num) {
    uint8_t pattern = SEG7_DISPLAY[num];
    
    // Set segments according to pattern
    HAL_GPIO_WritePin(a_v_GPIO_Port, a_v_Pin, (pattern & 0x01) ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(b_v_GPIO_Port, b_v_Pin, (pattern & 0x02) ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(c_v_GPIO_Port, c_v_Pin, (pattern & 0x04) ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(d_v_GPIO_Port, d_v_Pin, (pattern & 0x08) ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(e_v_GPIO_Port, e_v_Pin, (pattern & 0x10) ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(f_v_GPIO_Port, f_v_Pin, (pattern & 0x20) ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(g_v_GPIO_Port, g_v_Pin, (pattern & 0x40) ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

void display7SEG_horizontal(int num) {
    uint8_t pattern = SEG7_DISPLAY[num];
    
    // Set segments for horizontal 7-segment display
    HAL_GPIO_WritePin(a_h_GPIO_Port, a_h_Pin, (pattern & 0x01) ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(b_h_GPIO_Port, b_h_Pin, (pattern & 0x02) ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(c_h_GPIO_Port, c_h_Pin, (pattern & 0x04) ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(d_h_GPIO_Port, d_h_Pin, (pattern & 0x08) ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(e_h_GPIO_Port, e_h_Pin, (pattern & 0x10) ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(f_h_GPIO_Port, f_h_Pin, (pattern & 0x20) ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(g_h_GPIO_Port, g_h_Pin, (pattern & 0x40) ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

void update_clock_buffer(int index, int num) {
    if (index >= 0 && index < 4) {
        led_buffer[index] = num;
    }
}

void clearAllLed(void) {
    HAL_GPIO_WritePin(en0_GPIO_Port, en0_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(en1_GPIO_Port, en1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(en2_GPIO_Port, en2_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(en3_GPIO_Port, en3_Pin, GPIO_PIN_RESET);
}

void update_seven_segment_led(void) {
    // Turn off all LEDs
    clearAllLed();
    
    // Select the appropriate LED to display based on led_index
    switch (led_index) {
        case 0:
            // Display first digit for horizontal traffic light
            display7SEG_horizontal(led_buffer[0]);
            HAL_GPIO_WritePin(en0_GPIO_Port, en0_Pin, GPIO_PIN_SET);
            break;
        case 1:
            // Display second digit for horizontal traffic light
            display7SEG_horizontal(led_buffer[1]);
            HAL_GPIO_WritePin(en1_GPIO_Port, en1_Pin, GPIO_PIN_SET);
            break;
        case 2:
            // Display first digit for vertical traffic light
            display7SEG(led_buffer[2]);
            HAL_GPIO_WritePin(en2_GPIO_Port, en2_Pin, GPIO_PIN_SET);
            break;
        case 3:
            // Display second digit for vertical traffic light
            display7SEG(led_buffer[3]);
            HAL_GPIO_WritePin(en3_GPIO_Port, en3_Pin, GPIO_PIN_SET);
            break;
        default:
            break;
    }
    
    // Increment led_index for next update
    led_index = (led_index + 1) % 4;
}

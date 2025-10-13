/*
 * led_display.c
 *
 *  Created on: Oct 2, 2025
 *      Author: ADMIN
 */

#include "main.h"
#include "led_display.h"

// LED buffer for 7-segment display
int led_buffer[4] = {0, 0, 0, 0};
int index_led = 0;
int led_status = 0; // 0: normal, 1-3: blinking

// Default durations
int redDuration = 5;
int yellowDuration = 2;
int greenDuration = 3;

// 7-segment display patterns for 0-9 and OFF
int sevenSEG[11][7] = {
    {0, 0, 0, 0, 0, 0, 1}, // 0
    {1, 0, 0, 1, 1, 1, 1}, // 1
    {0, 0, 1, 0, 0, 1, 0}, // 2
    {0, 0, 0, 0, 1, 1, 0}, // 3
    {1, 0, 0, 1, 1, 0, 0}, // 4
    {0, 1, 0, 0, 1, 0, 0}, // 5
    {0, 1, 0, 0, 0, 0, 0}, // 6
    {0, 0, 0, 1, 1, 1, 1}, // 7
    {0, 0, 0, 0, 0, 0, 0}, // 8
    {0, 0, 0, 0, 1, 0, 0}, // 9
    {1, 1, 1, 1, 1, 1, 1}  // OFF - all segments off
};

// Display 7-segment for vertical road
void display7SEG_V(int num) {
    if (num >= 0 && num <= 9) {
        HAL_GPIO_WritePin(a_v_GPIO_Port, a_v_Pin, !sevenSEG[num][0]);
        HAL_GPIO_WritePin(b_v_GPIO_Port, b_v_Pin, !sevenSEG[num][1]);
        HAL_GPIO_WritePin(c_v_GPIO_Port, c_v_Pin, !sevenSEG[num][2]);
        HAL_GPIO_WritePin(d_v_GPIO_Port, d_v_Pin, !sevenSEG[num][3]);
        HAL_GPIO_WritePin(e_v_GPIO_Port, e_v_Pin, !sevenSEG[num][4]);
        HAL_GPIO_WritePin(f_v_GPIO_Port, f_v_Pin, !sevenSEG[num][5]);
        HAL_GPIO_WritePin(g_v_GPIO_Port, g_v_Pin, !sevenSEG[num][6]);
    } else {
        // Turn off all segments (set to HIGH for PNP transistors)
        HAL_GPIO_WritePin(a_v_GPIO_Port, a_v_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(b_v_GPIO_Port, b_v_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(c_v_GPIO_Port, c_v_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(d_v_GPIO_Port, d_v_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(e_v_GPIO_Port, e_v_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(f_v_GPIO_Port, f_v_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(g_v_GPIO_Port, g_v_Pin, GPIO_PIN_SET);
    }
}

// Display 7-segment for horizontal road
void display7SEG_H(int num) {
    if (num >= 0 && num <= 9) {
        HAL_GPIO_WritePin(a_h_GPIO_Port, a_h_Pin, !sevenSEG[num][0]);
        HAL_GPIO_WritePin(b_h_GPIO_Port, b_h_Pin, !sevenSEG[num][1]);
        HAL_GPIO_WritePin(c_h_GPIO_Port, c_h_Pin, !sevenSEG[num][2]);
        HAL_GPIO_WritePin(d_h_GPIO_Port, d_h_Pin, !sevenSEG[num][3]);
        HAL_GPIO_WritePin(e_h_GPIO_Port, e_h_Pin, !sevenSEG[num][4]);
        HAL_GPIO_WritePin(f_h_GPIO_Port, f_h_Pin, !sevenSEG[num][5]);
        HAL_GPIO_WritePin(g_h_GPIO_Port, g_h_Pin, !sevenSEG[num][6]);
    } else {
        // Turn off all segments
        HAL_GPIO_WritePin(a_h_GPIO_Port, a_h_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(b_h_GPIO_Port, b_h_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(c_h_GPIO_Port, c_h_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(d_h_GPIO_Port, d_h_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(e_h_GPIO_Port, e_h_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(f_h_GPIO_Port, f_h_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(g_h_GPIO_Port, g_h_Pin, GPIO_PIN_SET);
    }
}

// Update the 7-segment displays based on index
void update7SEG(int index) {
    // Turn off all LEDs (set to HIGH for PNP transistors)
    HAL_GPIO_WritePin(en0_GPIO_Port, en0_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(en1_GPIO_Port, en1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(en2_GPIO_Port, en2_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(en3_GPIO_Port, en3_Pin, GPIO_PIN_SET);
    
    switch(index) {
        case 0:
            // Turn on the first 7-SEG (set to LOW for PNP transistors)
            HAL_GPIO_WritePin(en0_GPIO_Port, en0_Pin, GPIO_PIN_RESET);
            display7SEG_H(led_buffer[0]);
            break;
        case 1:
            // Turn on the second 7-SEG
            HAL_GPIO_WritePin(en1_GPIO_Port, en1_Pin, GPIO_PIN_RESET);
            display7SEG_H(led_buffer[1]);
            break;
        case 2:
            // Turn on the third 7-SEG
            HAL_GPIO_WritePin(en2_GPIO_Port, en2_Pin, GPIO_PIN_RESET);
            display7SEG_V(led_buffer[2]);
            break;
        case 3:
            // Turn on the fourth 7-SEG
            HAL_GPIO_WritePin(en3_GPIO_Port, en3_Pin, GPIO_PIN_RESET);
            display7SEG_V(led_buffer[3]);
            break;
        default:
            break;
    }
}

// Clear all traffic lights
void clearAllTrafficLights() {
    // Turn off all lights (set to LOW for direct LED connection)
    HAL_GPIO_WritePin(red_h_GPIO_Port, red_h_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(yellow_h_GPIO_Port, yellow_h_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(green_h_GPIO_Port, green_h_Pin, GPIO_PIN_RESET);
    
    HAL_GPIO_WritePin(red_v_GPIO_Port, red_v_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(yellow_v_GPIO_Port, yellow_v_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(green_v_GPIO_Port, green_v_Pin, GPIO_PIN_RESET);
}

// Set traffic lights for both roads
// hLight/vLight: 0 = RED, 1 = YELLOW, 2 = GREEN
void setColorLights(int hLight, int vLight) {
    // Clear all lights first
    clearAllTrafficLights();
    
    // Set horizontal lights
    switch(hLight) {
        case 0: // RED
            HAL_GPIO_WritePin(red_h_GPIO_Port, red_h_Pin, GPIO_PIN_SET);
            break;
        case 1: // YELLOW
            HAL_GPIO_WritePin(yellow_h_GPIO_Port, yellow_h_Pin, GPIO_PIN_SET);
            break;
        case 2: // GREEN
            HAL_GPIO_WritePin(green_h_GPIO_Port, green_h_Pin, GPIO_PIN_SET);
            break;
    }
    
    // Set vertical lights
    switch(vLight) {
        case 0: // RED
            HAL_GPIO_WritePin(red_v_GPIO_Port, red_v_Pin, GPIO_PIN_SET);
            break;
        case 1: // YELLOW
            HAL_GPIO_WritePin(yellow_v_GPIO_Port, yellow_v_Pin, GPIO_PIN_SET);
            break;
        case 2: // GREEN
            HAL_GPIO_WritePin(green_v_GPIO_Port, green_v_Pin, GPIO_PIN_SET);
            break;
    }
}

// Blink all lights of the specified color
// colorType: 0 = RED, 1 = YELLOW, 2 = GREEN
void blinkAllLights(int colorType) {
    switch(colorType) {
        case 0: // RED
            HAL_GPIO_TogglePin(red_h_GPIO_Port, red_h_Pin);
            HAL_GPIO_TogglePin(red_v_GPIO_Port, red_v_Pin);
            break;
        case 1: // YELLOW
            HAL_GPIO_TogglePin(yellow_h_GPIO_Port, yellow_h_Pin);
            HAL_GPIO_TogglePin(yellow_v_GPIO_Port, yellow_v_Pin);
            break;
        case 2: // GREEN
            HAL_GPIO_TogglePin(green_h_GPIO_Port, green_h_Pin);
            HAL_GPIO_TogglePin(green_v_GPIO_Port, green_v_Pin);
            break;
    }
}
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

// Current light states
int horLightState = NONE;
int verLightState = NONE;

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
    {1, 1, 1, 1, 1, 1, 1}  // OFF
};

// Display 7-segment for vertical road
void display7SEG_V(int num) {
    if (num >= 0 && num <= 9) {
        HAL_GPIO_WritePin(a_v_GPIO_Port, a_v_Pin, sevenSEG[num][0]);
        HAL_GPIO_WritePin(b_v_GPIO_Port, b_v_Pin, sevenSEG[num][1]);
        HAL_GPIO_WritePin(c_v_GPIO_Port, c_v_Pin, sevenSEG[num][2]);
        HAL_GPIO_WritePin(d_v_GPIO_Port, d_v_Pin, sevenSEG[num][3]);
        HAL_GPIO_WritePin(e_v_GPIO_Port, e_v_Pin, sevenSEG[num][4]);
        HAL_GPIO_WritePin(f_v_GPIO_Port, f_v_Pin, sevenSEG[num][5]);
        HAL_GPIO_WritePin(g_v_GPIO_Port, g_v_Pin, sevenSEG[num][6]);
    } else {
        // Turn off all segments
        HAL_GPIO_WritePin(a_v_GPIO_Port, a_v_Pin, 1);
        HAL_GPIO_WritePin(b_v_GPIO_Port, b_v_Pin, 1);
        HAL_GPIO_WritePin(c_v_GPIO_Port, c_v_Pin, 1);
        HAL_GPIO_WritePin(d_v_GPIO_Port, d_v_Pin, 1);
        HAL_GPIO_WritePin(e_v_GPIO_Port, e_v_Pin, 1);
        HAL_GPIO_WritePin(f_v_GPIO_Port, f_v_Pin, 1);
        HAL_GPIO_WritePin(g_v_GPIO_Port, g_v_Pin, 1);
    }
}

// Display 7-segment for horizontal road
void display7SEG_H(int num) {
    if (num >= 0 && num <= 9) {
        HAL_GPIO_WritePin(a_h_GPIO_Port, a_h_Pin, sevenSEG[num][0]);
        HAL_GPIO_WritePin(b_h_GPIO_Port, b_h_Pin, sevenSEG[num][1]);
        HAL_GPIO_WritePin(c_h_GPIO_Port, c_h_Pin, sevenSEG[num][2]);
        HAL_GPIO_WritePin(d_h_GPIO_Port, d_h_Pin, sevenSEG[num][3]);
        HAL_GPIO_WritePin(e_h_GPIO_Port, e_h_Pin, sevenSEG[num][4]);
        HAL_GPIO_WritePin(f_h_GPIO_Port, f_h_Pin, sevenSEG[num][5]);
        HAL_GPIO_WritePin(g_h_GPIO_Port, g_h_Pin, sevenSEG[num][6]);
    } else {
        // Turn off all segments
        HAL_GPIO_WritePin(a_h_GPIO_Port, a_h_Pin, 1);
        HAL_GPIO_WritePin(b_h_GPIO_Port, b_h_Pin, 1);
        HAL_GPIO_WritePin(c_h_GPIO_Port, c_h_Pin, 1);
        HAL_GPIO_WritePin(d_h_GPIO_Port, d_h_Pin, 1);
        HAL_GPIO_WritePin(e_h_GPIO_Port, e_h_Pin, 1);
        HAL_GPIO_WritePin(f_h_GPIO_Port, f_h_Pin, 1);
        HAL_GPIO_WritePin(g_h_GPIO_Port, g_h_Pin, 1);
    }
}

// Update the 7-segment displays based on index
void update7SEG(int index) {
    // Turn off all LEDs first
    HAL_GPIO_WritePin(en0_GPIO_Port, en0_Pin, 1);
    HAL_GPIO_WritePin(en1_GPIO_Port, en1_Pin, 1);
    HAL_GPIO_WritePin(en2_GPIO_Port, en2_Pin, 1);
    HAL_GPIO_WritePin(en3_GPIO_Port, en3_Pin, 1);
    
    // Now turn on the selected one and display the value
    switch (index % 4) {
        case 0:
            HAL_GPIO_WritePin(en0_GPIO_Port, en0_Pin, 0);
            display7SEG_H(led_buffer[0]);
            break;
        case 1:
            HAL_GPIO_WritePin(en1_GPIO_Port, en1_Pin, 0);
            display7SEG_H(led_buffer[1]);
            break;
        case 2:
            HAL_GPIO_WritePin(en2_GPIO_Port, en2_Pin, 0);
            display7SEG_V(led_buffer[2]);
            break;
        case 3:
            HAL_GPIO_WritePin(en3_GPIO_Port, en3_Pin, 0);
            display7SEG_V(led_buffer[3]);
            break;
    }
}

// Clear all traffic lights
void clearAllTrafficLights() {
    // Turn off all lights
    HAL_GPIO_WritePin(red_h_GPIO_Port, red_h_Pin, LED_OFF);
    HAL_GPIO_WritePin(yellow_h_GPIO_Port, yellow_h_Pin, LED_OFF);
    HAL_GPIO_WritePin(green_h_GPIO_Port, green_h_Pin, LED_OFF);
    
    HAL_GPIO_WritePin(red_v_GPIO_Port, red_v_Pin, LED_OFF);
    HAL_GPIO_WritePin(yellow_v_GPIO_Port, yellow_v_Pin, LED_OFF);
    HAL_GPIO_WritePin(green_v_GPIO_Port, green_v_Pin, LED_OFF);
    
    horLightState = NONE;
    verLightState = NONE;
}

// Set traffic light color for a specific road
// road: 0 = horizontal, 1 = vertical
// color: 0 = RED, 1 = YELLOW, 2 = GREEN, 3 = ALL, 4 = NONE
void setTrafficLight(int road, int color) {
    if (road == 0) { // Horizontal
        switch (color) {
            case RED:
                HAL_GPIO_WritePin(red_h_GPIO_Port, red_h_Pin, LED_ON);
                HAL_GPIO_WritePin(yellow_h_GPIO_Port, yellow_h_Pin, LED_OFF);
                HAL_GPIO_WritePin(green_h_GPIO_Port, green_h_Pin, LED_OFF);
                horLightState = RED;
                break;
            case YELLOW:
                HAL_GPIO_WritePin(red_h_GPIO_Port, red_h_Pin, LED_OFF);
                HAL_GPIO_WritePin(yellow_h_GPIO_Port, yellow_h_Pin, LED_ON);
                HAL_GPIO_WritePin(green_h_GPIO_Port, green_h_Pin, LED_OFF);
                horLightState = YELLOW;
                break;
            case GREEN:
                HAL_GPIO_WritePin(red_h_GPIO_Port, red_h_Pin, LED_OFF);
                HAL_GPIO_WritePin(yellow_h_GPIO_Port, yellow_h_Pin, LED_OFF);
                HAL_GPIO_WritePin(green_h_GPIO_Port, green_h_Pin, LED_ON);
                horLightState = GREEN;
                break;
            case ALL_LED:
                HAL_GPIO_WritePin(red_h_GPIO_Port, red_h_Pin, LED_ON);
                HAL_GPIO_WritePin(yellow_h_GPIO_Port, yellow_h_Pin, LED_ON);
                HAL_GPIO_WritePin(green_h_GPIO_Port, green_h_Pin, LED_ON);
                horLightState = ALL_LED;
                break;
            default: // NONE
                HAL_GPIO_WritePin(red_h_GPIO_Port, red_h_Pin, LED_OFF);
                HAL_GPIO_WritePin(yellow_h_GPIO_Port, yellow_h_Pin, LED_OFF);
                HAL_GPIO_WritePin(green_h_GPIO_Port, green_h_Pin, LED_OFF);
                horLightState = NONE;
                break;
        }
    } else { // Vertical
        switch (color) {
            case RED:
                HAL_GPIO_WritePin(red_v_GPIO_Port, red_v_Pin, LED_ON);
                HAL_GPIO_WritePin(yellow_v_GPIO_Port, yellow_v_Pin, LED_OFF);
                HAL_GPIO_WritePin(green_v_GPIO_Port, green_v_Pin, LED_OFF);
                verLightState = RED;
                break;
            case YELLOW:
                HAL_GPIO_WritePin(red_v_GPIO_Port, red_v_Pin, LED_OFF);
                HAL_GPIO_WritePin(yellow_v_GPIO_Port, yellow_v_Pin, LED_ON);
                HAL_GPIO_WritePin(green_v_GPIO_Port, green_v_Pin, LED_OFF);
                verLightState = YELLOW;
                break;
            case GREEN:
                HAL_GPIO_WritePin(red_v_GPIO_Port, red_v_Pin, LED_OFF);
                HAL_GPIO_WritePin(yellow_v_GPIO_Port, yellow_v_Pin, LED_OFF);
                HAL_GPIO_WritePin(green_v_GPIO_Port, green_v_Pin, LED_ON);
                verLightState = GREEN;
                break;
            case ALL_LED:
                HAL_GPIO_WritePin(red_v_GPIO_Port, red_v_Pin, LED_ON);
                HAL_GPIO_WritePin(yellow_v_GPIO_Port, yellow_v_Pin, LED_ON);
                HAL_GPIO_WritePin(green_v_GPIO_Port, green_v_Pin, LED_ON);
                verLightState = ALL_LED;
                break;
            default: // NONE
                HAL_GPIO_WritePin(red_v_GPIO_Port, red_v_Pin, LED_OFF);
                HAL_GPIO_WritePin(yellow_v_GPIO_Port, yellow_v_Pin, LED_OFF);
                HAL_GPIO_WritePin(green_v_GPIO_Port, green_v_Pin, LED_OFF);
                verLightState = NONE;
                break;
        }
    }
}

// Toggle specific color of traffic lights for configuration modes
void toggleTrafficLight(int color) {
    switch (color) {
        case RED:
            HAL_GPIO_TogglePin(red_h_GPIO_Port, red_h_Pin);
            HAL_GPIO_TogglePin(red_v_GPIO_Port, red_v_Pin);
            break;
        case YELLOW:
            HAL_GPIO_TogglePin(yellow_h_GPIO_Port, yellow_h_Pin);
            HAL_GPIO_TogglePin(yellow_v_GPIO_Port, yellow_v_Pin);
            break;
        case GREEN:
            HAL_GPIO_TogglePin(green_h_GPIO_Port, green_h_Pin);
            HAL_GPIO_TogglePin(green_v_GPIO_Port, green_v_Pin);
            break;
    }
}

// Update LED buffer for both 7-segment displays
void updateLedBuffer(int hor_value, int ver_value) {
    led_buffer[0] = hor_value / 10;  // Tens place for horizontal
    led_buffer[1] = hor_value % 10;  // Ones place for horizontal
    led_buffer[2] = ver_value / 10;  // Tens place for vertical
    led_buffer[3] = ver_value % 10;  // Ones place for vertical
}
/*
 * led_display.c
 *
 *  Created on: Oct 2, 2025
 *      Author: ADMIN
 */

#include "main.h"
#include "led_display.h"

// 7-segment display patterns for digits 0-9 (0=on, 1=off)
// Format: a, b, c, d, e, f, g
const uint8_t SEG7_PATTERNS[10][7] = {
    {0, 0, 0, 0, 0, 0, 1}, // 0
    {1, 0, 0, 1, 1, 1, 1}, // 1
    {0, 0, 1, 0, 0, 1, 0}, // 2
    {0, 0, 0, 0, 1, 1, 0}, // 3
    {1, 0, 0, 1, 1, 0, 0}, // 4
    {0, 1, 0, 0, 1, 0, 0}, // 5
    {0, 1, 0, 0, 0, 0, 0}, // 6
    {0, 0, 0, 1, 1, 1, 1}, // 7
    {0, 0, 0, 0, 0, 0, 0}, // 8
    {0, 0, 0, 0, 1, 0, 0}  // 9
};

// Buffer to store values for 4 LEDs
int led_buffer[4] = {0, 0, 0, 0};
int led_index = 0;

// Time values for traffic lights (can be modified in modes 2-4)
int red_duration = 5;    // 5 seconds default
int yellow_duration = 2; // 2 seconds default
int green_duration = 3;  // 3 seconds default

// Current mode (1-4)
int current_mode = 1;

// Blinking flags and counters
int blink_flag = 0;
int blink_counter = 0;
int blink_duration = 5; // 5*10ms = 50ms for 2Hz blinking

// Traffic light state variables
int horizontal_light = 0; // 0=red, 1=yellow, 2=green
int vertical_light = 0;   // 0=red, 1=yellow, 2=green
int countdown_value = 0;  // Current countdown value

// Display a number on the horizontal 7-segment display
void display7SEG_H(int num) {
    if (num < 0 || num > 9) return;
    
    HAL_GPIO_WritePin(a_h_GPIO_Port, a_h_Pin, SEG7_PATTERNS[num][0]);
    HAL_GPIO_WritePin(b_h_GPIO_Port, b_h_Pin, SEG7_PATTERNS[num][1]);
    HAL_GPIO_WritePin(c_h_GPIO_Port, c_h_Pin, SEG7_PATTERNS[num][2]);
    HAL_GPIO_WritePin(d_h_GPIO_Port, d_h_Pin, SEG7_PATTERNS[num][3]);
    HAL_GPIO_WritePin(e_h_GPIO_Port, e_h_Pin, SEG7_PATTERNS[num][4]);
    HAL_GPIO_WritePin(f_h_GPIO_Port, f_h_Pin, SEG7_PATTERNS[num][5]);
    HAL_GPIO_WritePin(g_h_GPIO_Port, g_h_Pin, SEG7_PATTERNS[num][6]);
}

// Display a number on the vertical 7-segment display
void display7SEG_V(int num) {
    if (num < 0 || num > 9) return;
    
    HAL_GPIO_WritePin(a_v_GPIO_Port, a_v_Pin, SEG7_PATTERNS[num][0]);
    HAL_GPIO_WritePin(b_v_GPIO_Port, b_v_Pin, SEG7_PATTERNS[num][1]);
    HAL_GPIO_WritePin(c_v_GPIO_Port, c_v_Pin, SEG7_PATTERNS[num][2]);
    HAL_GPIO_WritePin(d_v_GPIO_Port, d_v_Pin, SEG7_PATTERNS[num][3]);
    HAL_GPIO_WritePin(e_v_GPIO_Port, e_v_Pin, SEG7_PATTERNS[num][4]);
    HAL_GPIO_WritePin(f_v_GPIO_Port, f_v_Pin, SEG7_PATTERNS[num][5]);
    HAL_GPIO_WritePin(g_v_GPIO_Port, g_v_Pin, SEG7_PATTERNS[num][6]);
}

// Enable one of the 4 7-segment displays at a time (for multiplexing)
void enable_7seg(int index) {
    switch (index) {
        case 0:
            HAL_GPIO_WritePin(en0_GPIO_Port, en0_Pin, GPIO_PIN_RESET); // Enable
            HAL_GPIO_WritePin(en1_GPIO_Port, en1_Pin, GPIO_PIN_SET);   // Disable
            HAL_GPIO_WritePin(en2_GPIO_Port, en2_Pin, GPIO_PIN_SET);   // Disable
            HAL_GPIO_WritePin(en3_GPIO_Port, en3_Pin, GPIO_PIN_SET);   // Disable
            break;
        case 1:
            HAL_GPIO_WritePin(en0_GPIO_Port, en0_Pin, GPIO_PIN_SET);   // Disable
            HAL_GPIO_WritePin(en1_GPIO_Port, en1_Pin, GPIO_PIN_RESET); // Enable
            HAL_GPIO_WritePin(en2_GPIO_Port, en2_Pin, GPIO_PIN_SET);   // Disable
            HAL_GPIO_WritePin(en3_GPIO_Port, en3_Pin, GPIO_PIN_SET);   // Disable
            break;
        case 2:
            HAL_GPIO_WritePin(en0_GPIO_Port, en0_Pin, GPIO_PIN_SET);   // Disable
            HAL_GPIO_WritePin(en1_GPIO_Port, en1_Pin, GPIO_PIN_SET);   // Disable
            HAL_GPIO_WritePin(en2_GPIO_Port, en2_Pin, GPIO_PIN_RESET); // Enable
            HAL_GPIO_WritePin(en3_GPIO_Port, en3_Pin, GPIO_PIN_SET);   // Disable
            break;
        case 3:
            HAL_GPIO_WritePin(en0_GPIO_Port, en0_Pin, GPIO_PIN_SET);   // Disable
            HAL_GPIO_WritePin(en1_GPIO_Port, en1_Pin, GPIO_PIN_SET);   // Disable
            HAL_GPIO_WritePin(en2_GPIO_Port, en2_Pin, GPIO_PIN_SET);   // Disable
            HAL_GPIO_WritePin(en3_GPIO_Port, en3_Pin, GPIO_PIN_RESET); // Enable
            break;
    }
}

// Update the 7-segment displays in a round-robin fashion
void update7SEG() {
    // Turn off all displays
    HAL_GPIO_WritePin(en0_GPIO_Port, en0_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(en1_GPIO_Port, en1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(en2_GPIO_Port, en2_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(en3_GPIO_Port, en3_Pin, GPIO_PIN_SET);
    
    // Update display based on current index
    switch (led_index) {
        case 0:
            // First horizontal digit (display ten's place of the value)
            display7SEG_H(led_buffer[0]);
            enable_7seg(0);
            break;
        case 1:
            // Second horizontal digit (display unit's place of the value)
            display7SEG_H(led_buffer[1]);
            enable_7seg(1);
            break;
        case 2:
            // First vertical digit (display ten's place of the value)
            display7SEG_V(led_buffer[2]);
            enable_7seg(2);
            break;
        case 3:
            // Second vertical digit (display unit's place of the value)
            display7SEG_V(led_buffer[3]);
            enable_7seg(3);
            break;
        default:
            break;
    }
    
    // Move to next display
    led_index = (led_index + 1) % 4;
}

// Update the LED buffer with the current values
void updateLEDBuffer(int time_value, int mode) {
    if (mode == 1) {
        // In normal mode, display countdown values
        led_buffer[0] = time_value / 10;  // Horizontal tens digit
        led_buffer[1] = time_value % 10;  // Horizontal ones digit
        led_buffer[2] = time_value / 10;  // Vertical tens digit
        led_buffer[3] = time_value % 10;  // Vertical ones digit
    } else {
        // In configuration modes, display mode in first 2 digits
        // and time value in last 2 digits
        led_buffer[0] = 0;
        led_buffer[1] = mode;
        
        // Get the duration value based on mode
        int duration = 0;
        switch (mode) {
            case 2: duration = red_duration; break;
            case 3: duration = yellow_duration; break;
            case 4: duration = green_duration; break;
            default: break;
        }
        
        led_buffer[2] = duration / 10;  // Tens digit
        led_buffer[3] = duration % 10;  // Ones digit
    }
}

// Clear all traffic lights
void clearAllLights() {
    HAL_GPIO_WritePin(red_h_GPIO_Port, red_h_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(yellow_h_GPIO_Port, yellow_h_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(green_h_GPIO_Port, green_h_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(red_v_GPIO_Port, red_v_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(yellow_v_GPIO_Port, yellow_v_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(green_v_GPIO_Port, green_v_Pin, GPIO_PIN_RESET);
}

// Toggle LEDs based on current mode for blinking effect
void toggleModeLEDs() {
    blink_counter++;
    if (blink_counter >= blink_duration) {
        blink_counter = 0;
        blink_flag = !blink_flag;
        
        switch (current_mode) {
            case 2: // Red LED modification mode
                HAL_GPIO_WritePin(red_h_GPIO_Port, red_h_Pin, blink_flag);
                HAL_GPIO_WritePin(red_v_GPIO_Port, red_v_Pin, blink_flag);
                break;
            case 3: // Yellow LED modification mode
                HAL_GPIO_WritePin(yellow_h_GPIO_Port, yellow_h_Pin, blink_flag);
                HAL_GPIO_WritePin(yellow_v_GPIO_Port, yellow_v_Pin, blink_flag);
                break;
            case 4: // Green LED modification mode
                HAL_GPIO_WritePin(green_h_GPIO_Port, green_h_Pin, blink_flag);
                HAL_GPIO_WritePin(green_v_GPIO_Port, green_v_Pin, blink_flag);
                break;
            default:
                // Normal mode - no blinking
                break;
        }
    }
}

// Display traffic lights based on current state
void displayTrafficLight() {
    if (current_mode == 1) {
        // Normal mode - implement the traffic light state machine
        // This is a simple implementation - you may need to adjust based on your requirements
        
        // Set horizontal traffic light
        HAL_GPIO_WritePin(red_h_GPIO_Port, red_h_Pin, horizontal_light == 0 ? GPIO_PIN_SET : GPIO_PIN_RESET);
        HAL_GPIO_WritePin(yellow_h_GPIO_Port, yellow_h_Pin, horizontal_light == 1 ? GPIO_PIN_SET : GPIO_PIN_RESET);
        HAL_GPIO_WritePin(green_h_GPIO_Port, green_h_Pin, horizontal_light == 2 ? GPIO_PIN_SET : GPIO_PIN_RESET);
        
        // Set vertical traffic light
        HAL_GPIO_WritePin(red_v_GPIO_Port, red_v_Pin, vertical_light == 0 ? GPIO_PIN_SET : GPIO_PIN_RESET);
        HAL_GPIO_WritePin(yellow_v_GPIO_Port, yellow_v_Pin, vertical_light == 1 ? GPIO_PIN_SET : GPIO_PIN_RESET);
        HAL_GPIO_WritePin(green_v_GPIO_Port, green_v_Pin, vertical_light == 2 ? GPIO_PIN_SET : GPIO_PIN_RESET);
    }
}

// Main update function
void updateDisplay() {
    updateLEDBuffer(countdown_value, current_mode);
    displayTrafficLight();
}

// Getters and setters for traffic light durations
void setRedDuration(int duration) {
    if (duration > 0 && duration <= 99) {
        red_duration = duration;
    }
}

void setYellowDuration(int duration) {
    if (duration > 0 && duration <= 99) {
        yellow_duration = duration;
    }
}

void setGreenDuration(int duration) {
    if (duration > 0 && duration <= 99) {
        green_duration = duration;
    }
}

int getRedDuration() {
    return red_duration;
}

int getYellowDuration() {
    return yellow_duration;
}

int getGreenDuration() {
    return green_duration;
}

// Mode handling
void setMode(int mode) {
    if (mode >= 1 && mode <= 4) {
        current_mode = mode;
    }
}

int getMode() {
    return current_mode;
}
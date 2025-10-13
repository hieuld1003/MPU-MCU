/*
 * led_display.c
 *
 *  Created on: Oct 2, 2025
 *      Author: ADMIN
 */

#include "main.h"
#include "led_display.h"

// Define the patterns for 7-segment display (0-9)
const int SEG7_DISPLAY[10][7] = {
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
int blink_duration = 25; // For 2Hz blinking (50 cycles = 500ms)

// Display a number (0-9) on a 7-segment display (horizontal or vertical)
void display7SEG_H(int num) {
    if (num >= 0 && num <= 9) {
        HAL_GPIO_WritePin(a_h_GPIO_Port, a_h_Pin, SEG7_DISPLAY[num][0]);
        HAL_GPIO_WritePin(b_h_GPIO_Port, b_h_Pin, SEG7_DISPLAY[num][1]);
        HAL_GPIO_WritePin(c_h_GPIO_Port, c_h_Pin, SEG7_DISPLAY[num][2]);
        HAL_GPIO_WritePin(d_h_GPIO_Port, d_h_Pin, SEG7_DISPLAY[num][3]);
        HAL_GPIO_WritePin(e_h_GPIO_Port, e_h_Pin, SEG7_DISPLAY[num][4]);
        HAL_GPIO_WritePin(f_h_GPIO_Port, f_h_Pin, SEG7_DISPLAY[num][5]);
        HAL_GPIO_WritePin(g_h_GPIO_Port, g_h_Pin, SEG7_DISPLAY[num][6]);
    }
}

void display7SEG_V(int num) {
    if (num >= 0 && num <= 9) {
        HAL_GPIO_WritePin(a_v_GPIO_Port, a_v_Pin, SEG7_DISPLAY[num][0]);
        HAL_GPIO_WritePin(b_v_GPIO_Port, b_v_Pin, SEG7_DISPLAY[num][1]);
        HAL_GPIO_WritePin(c_v_GPIO_Port, c_v_Pin, SEG7_DISPLAY[num][2]);
        HAL_GPIO_WritePin(d_v_GPIO_Port, d_v_Pin, SEG7_DISPLAY[num][3]);
        HAL_GPIO_WritePin(e_v_GPIO_Port, e_v_Pin, SEG7_DISPLAY[num][4]);
        HAL_GPIO_WritePin(f_v_GPIO_Port, f_v_Pin, SEG7_DISPLAY[num][5]);
        HAL_GPIO_WritePin(g_v_GPIO_Port, g_v_Pin, SEG7_DISPLAY[num][6]);
    }
}

// Enable specific display based on index
void enable_7seg(int index) {
    // Turn off all displays first
    HAL_GPIO_WritePin(en0_GPIO_Port, en0_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(en1_GPIO_Port, en1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(en2_GPIO_Port, en2_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(en3_GPIO_Port, en3_Pin, GPIO_PIN_SET);
    
    // Enable only the selected display (active low)
    switch (index) {
        case 0:
            HAL_GPIO_WritePin(en0_GPIO_Port, en0_Pin, GPIO_PIN_RESET);
            break;
        case 1:
            HAL_GPIO_WritePin(en1_GPIO_Port, en1_Pin, GPIO_PIN_RESET);
            break;
        case 2:
            HAL_GPIO_WritePin(en2_GPIO_Port, en2_Pin, GPIO_PIN_RESET);
            break;
        case 3:
            HAL_GPIO_WritePin(en3_GPIO_Port, en3_Pin, GPIO_PIN_RESET);
            break;
        default:
            break;
    }
}

// Update display on 7-segment LEDs
void update7SEG() {
    // Display the value in the buffer for current LED index
    if (led_index < 2) {
        // First two displays show the countdown value
        display7SEG_H(led_buffer[led_index]);
    } else {
        // Last two displays show the current mode
        display7SEG_V(led_buffer[led_index]);
    }
    
    // Enable the current display
    enable_7seg(led_index);
    
    // Move to next LED
    led_index = (led_index + 1) % 4;
}

// Update the LED buffer with countdown time and mode
void updateLEDBuffer(int time_value, int mode) {
    // First two digits for time value
    led_buffer[0] = time_value / 10;  // Tens digit
    led_buffer[1] = time_value % 10;  // Ones digit
    
    // Last two digits for mode
    led_buffer[2] = 0;  // Always 0 for mode (0-9)
    led_buffer[3] = mode;
}

// Set all traffic lights to off
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
                break;
        }
    }
}

// Handle LED display based on current mode
void displayTrafficLight() {
    if (current_mode == 1) {
        // Normal traffic light operation
        // Implementation of normal traffic light logic
        // This will depend on your traffic light FSM
    } else {
        // For configuration modes, display the appropriate time value
        int time_to_display = 0;
        
        switch (current_mode) {
            case 2:
                time_to_display = red_duration;
                break;
            case 3:
                time_to_display = yellow_duration;
                break;
            case 4:
                time_to_display = green_duration;
                break;
            default:
                break;
        }
        
        // Update the LED buffer with current time value and mode
        updateLEDBuffer(time_to_display, current_mode);
        
        // Handle blinking LEDs for the current mode
        toggleModeLEDs();
    }
}

// Function to be called periodically to update displays
void updateDisplay() {
    update7SEG();
    displayTrafficLight();
}
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
// Format: a, b, c, d, e, f, g
int arr[10][7] = {
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

// Traffic light states
int horizontalState = RED;
int verticalState = GREEN;

// Display-related variables
int displayIndex = 0;
int displayBuffer[4] = {0, 0, 0, 0};

void trafficLightInit(void) {
    // Set initial traffic light states
    // Horizontal: RED, Vertical: GREEN
    HAL_GPIO_WritePin(red_h_GPIO_Port, red_h_Pin, 0);       // RED ON
    HAL_GPIO_WritePin(yellow_h_GPIO_Port, yellow_h_Pin, 1);  // YELLOW OFF
    HAL_GPIO_WritePin(green_h_GPIO_Port, green_h_Pin, 1);    // GREEN OFF
    
    HAL_GPIO_WritePin(red_v_GPIO_Port, red_v_Pin, 1);        // RED OFF
    HAL_GPIO_WritePin(yellow_v_GPIO_Port, yellow_v_Pin, 1);  // YELLOW OFF
    HAL_GPIO_WritePin(green_v_GPIO_Port, green_v_Pin, 0);    // GREEN ON
    
    horizontalState = RED;
    verticalState = GREEN;
    horizontalCounter = redDuration;
    verticalCounter = greenDuration;
    
    // Initialize display buffer
    displayBuffer[0] = horizontalCounter / 10;
    displayBuffer[1] = horizontalCounter % 10;
    displayBuffer[2] = verticalCounter / 10;
    displayBuffer[3] = verticalCounter % 10;
}

void updateTrafficLights(void) {
    // Update horizontal traffic light
    horizontalCounter--;
    if (horizontalCounter <= 0) {
        switch (horizontalState) {
            case RED:
                horizontalState = GREEN;
                horizontalCounter = greenDuration;
                HAL_GPIO_WritePin(red_h_GPIO_Port, red_h_Pin, 1);     // RED OFF
                HAL_GPIO_WritePin(green_h_GPIO_Port, green_h_Pin, 0);  // GREEN ON
                break;
            case GREEN:
                horizontalState = AMBER;
                horizontalCounter = amberDuration;
                HAL_GPIO_WritePin(green_h_GPIO_Port, green_h_Pin, 1);   // GREEN OFF
                HAL_GPIO_WritePin(yellow_h_GPIO_Port, yellow_h_Pin, 0); // YELLOW ON
                break;
            case AMBER:
                horizontalState = RED;
                horizontalCounter = redDuration;
                HAL_GPIO_WritePin(yellow_h_GPIO_Port, yellow_h_Pin, 1); // YELLOW OFF
                HAL_GPIO_WritePin(red_h_GPIO_Port, red_h_Pin, 0);       // RED ON
                break;
        }
    }
    
    // Update vertical traffic light
    verticalCounter--;
    if (verticalCounter <= 0) {
        switch (verticalState) {
            case RED:
                verticalState = GREEN;
                verticalCounter = greenDuration;
                HAL_GPIO_WritePin(red_v_GPIO_Port, red_v_Pin, 1);      // RED OFF
                HAL_GPIO_WritePin(green_v_GPIO_Port, green_v_Pin, 0);   // GREEN ON
                break;
            case GREEN:
                verticalState = AMBER;
                verticalCounter = amberDuration;
                HAL_GPIO_WritePin(green_v_GPIO_Port, green_v_Pin, 1);   // GREEN OFF
                HAL_GPIO_WritePin(yellow_v_GPIO_Port, yellow_v_Pin, 0); // YELLOW ON
                break;
            case AMBER:
                verticalState = RED;
                verticalCounter = redDuration;
                HAL_GPIO_WritePin(yellow_v_GPIO_Port, yellow_v_Pin, 1); // YELLOW OFF
                HAL_GPIO_WritePin(red_v_GPIO_Port, red_v_Pin, 0);       // RED ON
                break;
        }
    }
    
    // Update display buffer for countdown
    displayBuffer[0] = horizontalCounter / 10;
    displayBuffer[1] = horizontalCounter % 10;
    displayBuffer[2] = verticalCounter / 10;
    displayBuffer[3] = verticalCounter % 10;
}

void blinkRedLeds(void) {
    if (timerFlag(&timerForBlink)) {
        HAL_GPIO_TogglePin(red_h_GPIO_Port, red_h_Pin);
        HAL_GPIO_TogglePin(red_v_GPIO_Port, red_v_Pin);
        setTimer(&timerForBlink, 500); // 0.5s (2Hz)
    }
}

void blinkAmberLeds(void) {
    if (timerFlag(&timerForBlink)) {
        HAL_GPIO_TogglePin(yellow_h_GPIO_Port, yellow_h_Pin);
        HAL_GPIO_TogglePin(yellow_v_GPIO_Port, yellow_v_Pin);
        setTimer(&timerForBlink, 500); // 0.5s (2Hz)
    }
}

void blinkGreenLeds(void) {
    if (timerFlag(&timerForBlink)) {
        HAL_GPIO_TogglePin(green_h_GPIO_Port, green_h_Pin);
        HAL_GPIO_TogglePin(green_v_GPIO_Port, green_v_Pin);
        setTimer(&timerForBlink, 500); // 0.5s (2Hz)
    }
}

void clearBlinkingLeds(void) {
    HAL_GPIO_WritePin(red_h_GPIO_Port, red_h_Pin, 1);      // OFF
    HAL_GPIO_WritePin(yellow_h_GPIO_Port, yellow_h_Pin, 1); // OFF
    HAL_GPIO_WritePin(green_h_GPIO_Port, green_h_Pin, 1);   // OFF
    HAL_GPIO_WritePin(red_v_GPIO_Port, red_v_Pin, 1);       // OFF
    HAL_GPIO_WritePin(yellow_v_GPIO_Port, yellow_v_Pin, 1); // OFF
    HAL_GPIO_WritePin(green_v_GPIO_Port, green_v_Pin, 1);   // OFF
}

void turnOffAllDisplay(void) {
    HAL_GPIO_WritePin(en0_GPIO_Port, en0_Pin, 0);
    HAL_GPIO_WritePin(en1_GPIO_Port, en1_Pin, 0);
    HAL_GPIO_WritePin(en2_GPIO_Port, en2_Pin, 0);
    HAL_GPIO_WritePin(en3_GPIO_Port, en3_Pin, 0);
}

void selectDisplay(int index) {
    turnOffAllDisplay();
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
    }
}

void display7SEG_h(int num) {
    if (num >= 0 && num <= 9) {
        // Control horizontal 7-segment using individual pins
        HAL_GPIO_WritePin(a_h_GPIO_Port, a_h_Pin, arr[num][0]);
        HAL_GPIO_WritePin(b_h_GPIO_Port, b_h_Pin, arr[num][1]);
        HAL_GPIO_WritePin(c_h_GPIO_Port, c_h_Pin, arr[num][2]);
        HAL_GPIO_WritePin(d_h_GPIO_Port, d_h_Pin, arr[num][3]);
        HAL_GPIO_WritePin(e_h_GPIO_Port, e_h_Pin, arr[num][4]);
        HAL_GPIO_WritePin(f_h_GPIO_Port, f_h_Pin, arr[num][5]);
        HAL_GPIO_WritePin(g_h_GPIO_Port, g_h_Pin, arr[num][6]);
    }
}

void display7SEG_v(int num) {
    if (num >= 0 && num <= 9) {
        // Control vertical 7-segment using individual pins
        HAL_GPIO_WritePin(a_v_GPIO_Port, a_v_Pin, arr[num][0]);
        HAL_GPIO_WritePin(b_v_GPIO_Port, b_v_Pin, arr[num][1]);
        HAL_GPIO_WritePin(c_v_GPIO_Port, c_v_Pin, arr[num][2]);
        HAL_GPIO_WritePin(d_v_GPIO_Port, d_v_Pin, arr[num][3]);
        HAL_GPIO_WritePin(e_v_GPIO_Port, e_v_Pin, arr[num][4]);
        HAL_GPIO_WritePin(f_v_GPIO_Port, f_v_Pin, arr[num][5]);
        HAL_GPIO_WritePin(g_v_GPIO_Port, g_v_Pin, arr[num][6]);
    }
}

void updateDisplays(void) {
    // Cycle through displays for multiplexing
    static int displayCount = 0;
    
    // Move to next display
    displayCount = (displayCount + 1) % 4;
    selectDisplay(displayCount);
    
    // Display appropriate digit based on mode
    if (currentMode == MODE_NORMAL) {
        // Normal mode - show counters
        if (displayCount < 2) {
            // Horizontal displays (0, 1)
            display7SEG_h(displayBuffer[displayCount]);
        } else {
            // Vertical displays (2, 3)
            display7SEG_v(displayBuffer[displayCount]);
        }
    } else {
        // Configuration mode
        if (displayCount < 2) {
            // First two displays show the value being configured
            if (displayCount == 0) {
                display7SEG_h(tempValue / 10);
            } else {
                display7SEG_h(tempValue % 10);
            }
        } else {
            // Last two displays show the mode number
            if (displayCount == 2) {
                display7SEG_v(0);
            } else {
                display7SEG_v(currentMode);
            }
        }
    }
}
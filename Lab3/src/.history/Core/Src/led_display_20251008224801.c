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

// 7-segment LED segments encoding (0-9)
// Format: a, b, c, d, e, f, g
int segmentData[10] = {
    0x3F, // 0: 0b00111111
    0x06, // 1: 0b00000110
    0x5B, // 2: 0b01011011
    0x4F, // 3: 0b01001111
    0x66, // 4: 0b01100110
    0x6D, // 5: 0b01101101
    0x7D, // 6: 0b01111101
    0x07, // 7: 0b00000111
    0x7F, // 8: 0b01111111
    0x6F  // 9: 0b01101111
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
    HAL_GPIO_WritePin(red_h_GPIO_Port, red_h_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(yellow_h_GPIO_Port, yellow_h_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(green_h_GPIO_Port, green_h_Pin, GPIO_PIN_RESET);
    
    HAL_GPIO_WritePin(red_v_GPIO_Port, red_v_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(yellow_v_GPIO_Port, yellow_v_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(green_v_GPIO_Port, green_v_Pin, GPIO_PIN_SET);
    
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
                HAL_GPIO_WritePin(red_h_GPIO_Port, red_h_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(green_h_GPIO_Port, green_h_Pin, GPIO_PIN_SET);
                break;
            case GREEN:
                horizontalState = AMBER;
                horizontalCounter = amberDuration;
                HAL_GPIO_WritePin(green_h_GPIO_Port, green_h_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(yellow_h_GPIO_Port, yellow_h_Pin, GPIO_PIN_SET);
                break;
            case AMBER:
                horizontalState = RED;
                horizontalCounter = redDuration;
                HAL_GPIO_WritePin(yellow_h_GPIO_Port, yellow_h_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(red_h_GPIO_Port, red_h_Pin, GPIO_PIN_SET);
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
                HAL_GPIO_WritePin(red_v_GPIO_Port, red_v_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(green_v_GPIO_Port, green_v_Pin, GPIO_PIN_SET);
                break;
            case GREEN:
                verticalState = AMBER;
                verticalCounter = amberDuration;
                HAL_GPIO_WritePin(green_v_GPIO_Port, green_v_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(yellow_v_GPIO_Port, yellow_v_Pin, GPIO_PIN_SET);
                break;
            case AMBER:
                verticalState = RED;
                verticalCounter = redDuration;
                HAL_GPIO_WritePin(yellow_v_GPIO_Port, yellow_v_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(red_v_GPIO_Port, red_v_Pin, GPIO_PIN_SET);
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
    static int blinkState = 0;
    
    if (timerFlag(&timerForBlink)) {
        blinkState = !blinkState;
        HAL_GPIO_WritePin(red_h_GPIO_Port, red_h_Pin, blinkState ? GPIO_PIN_SET : GPIO_PIN_RESET);
        HAL_GPIO_WritePin(red_v_GPIO_Port, red_v_Pin, blinkState ? GPIO_PIN_SET : GPIO_PIN_RESET);
        setTimer(&timerForBlink, 500); // 0.5s (2Hz)
    }
}

void blinkAmberLeds(void) {
    static int blinkState = 0;
    
    if (timerFlag(&timerForBlink)) {
        blinkState = !blinkState;
        HAL_GPIO_WritePin(yellow_h_GPIO_Port, yellow_h_Pin, blinkState ? GPIO_PIN_SET : GPIO_PIN_RESET);
        HAL_GPIO_WritePin(yellow_v_GPIO_Port, yellow_v_Pin, blinkState ? GPIO_PIN_SET : GPIO_PIN_RESET);
        setTimer(&timerForBlink, 500); // 0.5s (2Hz)
    }
}

void blinkGreenLeds(void) {
    static int blinkState = 0;
    
    if (timerFlag(&timerForBlink)) {
        blinkState = !blinkState;
        HAL_GPIO_WritePin(green_h_GPIO_Port, green_h_Pin, blinkState ? GPIO_PIN_SET : GPIO_PIN_RESET);
        HAL_GPIO_WritePin(green_v_GPIO_Port, green_v_Pin, blinkState ? GPIO_PIN_SET : GPIO_PIN_RESET);
        setTimer(&timerForBlink, 500); // 0.5s (2Hz)
    }
}

void clearBlinkingLeds(void) {
    HAL_GPIO_WritePin(red_h_GPIO_Port, red_h_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(yellow_h_GPIO_Port, yellow_h_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(green_h_GPIO_Port, green_h_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(red_v_GPIO_Port, red_v_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(yellow_v_GPIO_Port, yellow_v_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(green_v_GPIO_Port, green_v_Pin, GPIO_PIN_RESET);
}

void turnOffAllDisplay(void) {
    HAL_GPIO_WritePin(en0_GPIO_Port, en0_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(en1_GPIO_Port, en1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(en2_GPIO_Port, en2_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(en3_GPIO_Port, en3_Pin, GPIO_PIN_RESET);
}

void selectDisplay(int index) {
    turnOffAllDisplay();
    switch (index) {
        case 0:
            HAL_GPIO_WritePin(en0_GPIO_Port, en0_Pin, GPIO_PIN_SET);
            break;
        case 1:
            HAL_GPIO_WritePin(en1_GPIO_Port, en1_Pin, GPIO_PIN_SET);
            break;
        case 2:
            HAL_GPIO_WritePin(en2_GPIO_Port, en2_Pin, GPIO_PIN_SET);
            break;
        case 3:
            HAL_GPIO_WritePin(en3_GPIO_Port, en3_Pin, GPIO_PIN_SET);
            break;
    }
}

void displaySegment(int num, int isVertical) {
    if (num < 0 || num > 9) return;
    
    int value = segmentData[num];
    
    if (isVertical) {
        // Vertical 7-segment display
        HAL_GPIO_WritePin(a_v_GPIO_Port, a_v_Pin, (value & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
        HAL_GPIO_WritePin(b_v_GPIO_Port, b_v_Pin, (value & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET);
        HAL_GPIO_WritePin(c_v_GPIO_Port, c_v_Pin, (value & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET);
        HAL_GPIO_WritePin(d_v_GPIO_Port, d_v_Pin, (value & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET);
        HAL_GPIO_WritePin(e_v_GPIO_Port, e_v_Pin, (value & 0x10) ? GPIO_PIN_SET : GPIO_PIN_RESET);
        HAL_GPIO_WritePin(f_v_GPIO_Port, f_v_Pin, (value & 0x20) ? GPIO_PIN_SET : GPIO_PIN_RESET);
        HAL_GPIO_WritePin(g_v_GPIO_Port, g_v_Pin, (value & 0x40) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    } else {
        // Horizontal 7-segment display
        HAL_GPIO_WritePin(a_h_GPIO_Port, a_h_Pin, (value & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
        HAL_GPIO_WritePin(b_h_GPIO_Port, b_h_Pin, (value & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET);
        HAL_GPIO_WritePin(c_h_GPIO_Port, c_h_Pin, (value & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET);
        HAL_GPIO_WritePin(d_h_GPIO_Port, d_h_Pin, (value & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET);
        HAL_GPIO_WritePin(e_h_GPIO_Port, e_h_Pin, (value & 0x10) ? GPIO_PIN_SET : GPIO_PIN_RESET);
        HAL_GPIO_WritePin(f_h_GPIO_Port, f_h_Pin, (value & 0x20) ? GPIO_PIN_SET : GPIO_PIN_RESET);
        HAL_GPIO_WritePin(g_h_GPIO_Port, g_h_Pin, (value & 0x40) ? GPIO_PIN_SET : GPIO_PIN_RESET);
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
            displaySegment(displayBuffer[displayCount], 0);
        } else {
            // Vertical displays (2, 3)
            displaySegment(displayBuffer[displayCount], 1);
        }
    } else {
        // Configuration mode
        if (displayCount < 2) {
            // First two displays show the value being configured
            if (displayCount == 0) {
                displaySegment(tempValue / 10, 0);
            } else {
                displaySegment(tempValue % 10, 0);
            }
        } else {
            // Last two displays show the mode number
            if (displayCount == 2) {
                displaySegment(0, 1);
            } else {
                displaySegment(currentMode, 1);
            }
        }
    }
}
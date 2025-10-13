/*
 * input_processing.c
 *
 *  Created on: Oct 2, 2025
 *      Author: ADMIN
 */

#include "main.h"
#include "input_reading.h"
#include "input_processing.h"
#include "led_display.h"
#include "timer.h"

// System state
int systemMode = INIT;
int trafficState = HOR_GREEN_VER_RED;
int horCounter = 0;
int verCounter = 0;
int tempValue = 0;
int scan = 0;

// Update LED display based on current counter values
void updateSegmentDisplay() {
    updateLedBuffer(horCounter, verCounter);
}

// Handle button press for mode change
void handleModeButton() {
    if (is_button_pressed(0)) {
        // In NORMAL_MODE, go to RED_CONFIG
        if (systemMode == NORMAL_MODE) {
            resetAllTimers();
            systemMode = RED_CONFIG;
            tempValue = redDuration;
            setTimer1(500);  // Blinking timer
            setTimer3(100);  // Scanning timer
            clearAllTrafficLights();
        }
        // In RED_CONFIG, go to YELLOW_CONFIG
        else if (systemMode == RED_CONFIG) {
            systemMode = YELLOW_CONFIG;
            tempValue = yellowDuration;
            setTimer1(500);  // Blinking timer
            setTimer3(100);  // Scanning timer
            clearAllTrafficLights();
        }
        // In YELLOW_CONFIG, go to GREEN_CONFIG
        else if (systemMode == YELLOW_CONFIG) {
            systemMode = GREEN_CONFIG;
            tempValue = greenDuration;
            setTimer1(500);  // Blinking timer
            setTimer3(100);  // Scanning timer
            clearAllTrafficLights();
        }
        // In GREEN_CONFIG, go back to NORMAL_MODE
        else if (systemMode == GREEN_CONFIG) {
            // Validate configuration values
            if (yellowDuration > greenDuration) {
                int temp = yellowDuration;
                yellowDuration = greenDuration;
                greenDuration = temp;
            }
            if (redDuration < greenDuration + yellowDuration) {
                redDuration = greenDuration + yellowDuration;
            }
            
            resetAllTimers();
            systemMode = NORMAL_MODE;
            trafficState = HOR_GREEN_VER_RED;
            horCounter = greenDuration;
            verCounter = redDuration;
            setTimer1(1000);  // Timer for countdown
            setTimer2(1000);  // Timer for state change
            setTimer3(100);   // Scanning timer
            setTrafficLight(0, GREEN);
            setTrafficLight(1, RED);
        }
    }
}

// Handle button press for value increment
void handleIncrementButton() {
    if (systemMode >= RED_CONFIG && systemMode <= GREEN_CONFIG) {
        if (is_button_pressed(1)) {
            tempValue++;
            if (tempValue > 99) tempValue = 1;
        }
        else if (is_button_pressed_1s(1)) {
            if (timer4_flag) {
                tempValue++;
                if (tempValue > 99) tempValue = 1;
                setTimer4(100);
            }
        }
    }
}

// Handle button press for value confirmation
void handleConfirmButton() {
    if (systemMode >= RED_CONFIG && systemMode <= GREEN_CONFIG) {
        if (is_button_pressed(2)) {
            switch (systemMode) {
                case RED_CONFIG:
                    redDuration = tempValue;
                    break;
                case YELLOW_CONFIG:
                    yellowDuration = tempValue;
                    break;
                case GREEN_CONFIG:
                    greenDuration = tempValue;
                    break;
            }
        }
    }
}

void fsm_for_input_processing(void) {
    // Handle button presses
    handleModeButton();
    handleIncrementButton();
    handleConfirmButton();

    // Process based on system mode
    switch (systemMode) {
        case INIT:
            systemMode = NORMAL_MODE;
            trafficState = HOR_GREEN_VER_RED;
            horCounter = greenDuration;
            verCounter = redDuration;
            setTimer1(1000);  // Timer for countdown
            setTimer2(1000);  // Timer for state change
            setTimer3(100);   // Scanning timer
            setTrafficLight(0, GREEN);
            setTrafficLight(1, RED);
            break;
            
        case NORMAL_MODE:
            // Update counters
            if (timer1_flag) {
                horCounter--;
                verCounter--;
                updateSegmentDisplay();
                setTimer1(1000);
                
                // Check if time to change state
                if (horCounter <= 0 || verCounter <= 0) {
                    switch (trafficState) {
                        case HOR_GREEN_VER_RED:
                            trafficState = HOR_YELLOW_VER_RED;
                            horCounter = yellowDuration;
                            verCounter = redDuration - greenDuration;
                            setTrafficLight(0, YELLOW);
                            setTrafficLight(1, RED);
                            break;
                            
                        case HOR_YELLOW_VER_RED:
                            trafficState = HOR_RED_VER_GREEN;
                            horCounter = redDuration;
                            verCounter = greenDuration;
                            setTrafficLight(0, RED);
                            setTrafficLight(1, GREEN);
                            break;
                            
                        case HOR_RED_VER_GREEN:
                            trafficState = HOR_RED_VER_YELLOW;
                            horCounter = redDuration - greenDuration;
                            verCounter = yellowDuration;
                            setTrafficLight(0, RED);
                            setTrafficLight(1, YELLOW);
                            break;
                            
                        case HOR_RED_VER_YELLOW:
                            trafficState = HOR_GREEN_VER_RED;
                            horCounter = greenDuration;
                            verCounter = redDuration;
                            setTrafficLight(0, GREEN);
                            setTrafficLight(1, RED);
                            break;
                    }
                }
            }
            
            // Update display
            if (timer3_flag) {
                scan = (scan + 1) % 4;
                update7SEG(scan);
                setTimer3(100);
            }
            break;
            
        case RED_CONFIG:
            // Update display for configuration mode
            updateLedBuffer(0, 1);  // Display mode number "01"
            led_buffer[2] = tempValue / 10;
            led_buffer[3] = tempValue % 10;
            
            // Blink red LEDs
            if (timer1_flag) {
                toggleTrafficLight(RED);
                setTimer1(500);
            }
            
            // Update display
            if (timer3_flag) {
                scan = (scan + 1) % 4;
                update7SEG(scan);
                setTimer3(100);
            }
            break;
            
        case YELLOW_CONFIG:
            // Update display for configuration mode
            updateLedBuffer(0, 2);  // Display mode number "02"
            led_buffer[2] = tempValue / 10;
            led_buffer[3] = tempValue % 10;
            
            // Blink yellow LEDs
            if (timer1_flag) {
                toggleTrafficLight(YELLOW);
                setTimer1(500);
            }
            
            // Update display
            if (timer3_flag) {
                scan = (scan + 1) % 4;
                update7SEG(scan);
                setTimer3(100);
            }
            break;
            
        case GREEN_CONFIG:
            // Update display for configuration mode
            updateLedBuffer(0, 3);  // Display mode number "03"
            led_buffer[2] = tempValue / 10;
            led_buffer[3] = tempValue % 10;
            
            // Blink green LEDs
            if (timer1_flag) {
                toggleTrafficLight(GREEN);
                setTimer1(500);
            }
            
            // Update display
            if (timer3_flag) {
                scan = (scan + 1) % 4;
                update7SEG(scan);
                setTimer3(100);
            }
            break;
    }
}
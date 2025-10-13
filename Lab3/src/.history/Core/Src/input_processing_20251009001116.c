/*
 * input_processing.c
 *
 *  Created on: Oct 2, 2025
 *      Author: ADMIN
 */

#include "main.h"
#include "input_processing.h"
#include "input_reading.h"
#include "led_display.h"
#include "timer.h"

// Global variables
int status = INIT;
int displayBuffer[4] = {0};
int configValue = 0;

// FSM for button processing
enum ButtonState {BUTTON_RELEASED, BUTTON_PRESSED, BUTTON_PRESSED_MORE_THAN_1_SECOND};
enum ButtonState buttonState[3] = {BUTTON_RELEASED, BUTTON_RELEASED, BUTTON_RELEASED};

// Function to handle the traffic light operation
void fsm_automatic() {
    static int counter1 = 0;
    static int counter2 = 0;
    static int horizontal_state = RED;
    static int vertical_state = GREEN;
    
    if (timer1_flag == 1) {
        setTimer1(1000); // Update every 1s
        
        if (counter1 > 0) counter1--;
        if (counter2 > 0) counter2--;
        
        // Update horizontal traffic light
        if (counter1 == 0) {
            if (horizontal_state == RED) {
                horizontal_state = GREEN;
                counter1 = greenDuration;
            } else if (horizontal_state == GREEN) {
                horizontal_state = AMBER;
                counter1 = amberDuration;
            } else {
                horizontal_state = RED;
                counter1 = redDuration;
            }
        }
        
        // Update vertical traffic light
        if (counter2 == 0) {
            if (vertical_state == RED) {
                vertical_state = GREEN;
                counter2 = greenDuration;
            } else if (vertical_state == GREEN) {
                vertical_state = AMBER;
                counter2 = amberDuration;
            } else {
                vertical_state = RED;
                counter2 = redDuration;
            }
        }
        
        // Update traffic lights
        setTrafficLight(horizontal_state, vertical_state);
        
        // Update counters
        horizontalCounter = counter1;
        verticalCounter = counter2;
        updateClockBuffer();
    }
}

// Initialize traffic light system
void initialize() {
    status = NORMAL_MODE;
    horizontalCounter = redDuration;
    verticalCounter = greenDuration;
    setTrafficLight(RED, GREEN);
    updateClockBuffer();
    setTimer1(1000); // Start 1s timer for traffic light
    setTimer2(1);    // Start display scanning immediately
}

void fsm_for_input_processing() {
    // Scanning 7-segment displays is always active
    scan7Seg();
    
    // Process according to current system status
    switch (status) {
        case INIT:
            initialize();
            break;
            
        case NORMAL_MODE:
            fsm_automatic();
            processButtonForMode();
            break;
            
        case RED_MODE:
            clearAllLights();
            blinkAllRed();
            processButtonForMode();
            processButtonForInc();
            processButtonForSet();
            break;
            
        case AMBER_MODE:
            clearAllLights();
            blinkAllYellow();
            processButtonForMode();
            processButtonForInc();
            processButtonForSet();
            break;
            
        case GREEN_MODE:
            clearAllLights();
            blinkAllGreen();
            processButtonForMode();
            processButtonForInc();
            processButtonForSet();
            break;
            
        default:
            break;
    }
}

// Process button for changing modes
void processButtonForMode() {
    switch (buttonState[0]) {
        case BUTTON_RELEASED:
            if (is_button_pressed(0)) {
                buttonState[0] = BUTTON_PRESSED;
                
                // Change mode
                status = (status + 1) % 5;
                if (status == 0) status = 1; // Skip INIT state
                
                // Set initial values for configuration modes
                if (status == RED_MODE) {
                    configValue = redDuration;
                    setTimer2(1); // Start blinking immediately
                } else if (status == AMBER_MODE) {
                    configValue = amberDuration;
                    setTimer2(1); // Start blinking immediately
                } else if (status == GREEN_MODE) {
                    configValue = greenDuration;
                    setTimer2(1); // Start blinking immediately
                } else if (status == NORMAL_MODE) {
                    initialize();
                }
            }
            break;
            
        case BUTTON_PRESSED:
            if (!is_button_pressed(0)) {
                buttonState[0] = BUTTON_RELEASED;
            } else if (is_button_pressed_1s(0)) {
                buttonState[0] = BUTTON_PRESSED_MORE_THAN_1_SECOND;
            }
            break;
            
        case BUTTON_PRESSED_MORE_THAN_1_SECOND:
            if (!is_button_pressed(0)) {
                buttonState[0] = BUTTON_RELEASED;
            }
            break;
    }
}

// Process button for incrementing values
void processButtonForInc() {
    switch (buttonState[1]) {
        case BUTTON_RELEASED:
            if (is_button_pressed(1)) {
                buttonState[1] = BUTTON_PRESSED;
                
                // Increment configuration value
                configValue++;
                if (configValue > 99) configValue = 1;
            }
            break;
            
        case BUTTON_PRESSED:
            if (!is_button_pressed(1)) {
                buttonState[1] = BUTTON_RELEASED;
            } else if (is_button_pressed_1s(1)) {
                buttonState[1] = BUTTON_PRESSED_MORE_THAN_1_SECOND;
                setTimer3(1); // Start auto-increment immediately
            }
            break;
            
        case BUTTON_PRESSED_MORE_THAN_1_SECOND:
            if (!is_button_pressed(1)) {
                buttonState[1] = BUTTON_RELEASED;
            } else {
                // Auto-increment
                if (timer3_flag == 1) {
                    setTimer3(500); // Auto-increment every 0.5s
                    configValue++;
                    if (configValue > 99) configValue = 1;
                }
            }
            break;
    }
}

// Process button for setting values
void processButtonForSet() {
    switch (buttonState[2]) {
        case BUTTON_RELEASED:
            if (is_button_pressed(2)) {
                buttonState[2] = BUTTON_PRESSED;
                
                // Set configuration value
                if (status == RED_MODE) {
                    redDuration = configValue;
                } else if (status == AMBER_MODE) {
                    amberDuration = configValue;
                } else if (status == GREEN_MODE) {
                    greenDuration = configValue;
                }
            }
            break;
            
        case BUTTON_PRESSED:
            if (!is_button_pressed(2)) {
                buttonState[2] = BUTTON_RELEASED;
            } else if (is_button_pressed_1s(2)) {
                buttonState[2] = BUTTON_PRESSED_MORE_THAN_1_SECOND;
            }
            break;
            
        case BUTTON_PRESSED_MORE_THAN_1_SECOND:
            if (!is_button_pressed(2)) {
                buttonState[2] = BUTTON_RELEASED;
            }
            break;
    }
}
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

// Button states
enum ButtonState {BUTTON_RELEASED, BUTTON_PRESSED, BUTTON_PRESSED_MORE_THAN_1_SECOND};
enum ButtonState buttonState[3] = {BUTTON_RELEASED, BUTTON_RELEASED, BUTTON_RELEASED};

// System state
int systemState = INIT;
int trafficState = H_GREEN_V_RED;
int timeCounter = 0;
int tempValue = 0;
int blinkFlag = 0;

// Update LED display buffers for normal mode
void updateNormalModeDisplay() {
    // Update based on traffic state
    switch(trafficState) {
        case H_GREEN_V_RED:
            led_buffer[0] = timeCounter / 10; // Tens digit for horizontal
            led_buffer[1] = timeCounter % 10; // Ones digit for horizontal
            led_buffer[2] = redDuration - timeCounter > 9 ? (redDuration - timeCounter) / 10 : 0; // Tens digit for vertical
            led_buffer[3] = (redDuration - timeCounter) % 10; // Ones digit for vertical
            break;
            
        case H_YELLOW_V_RED:
            led_buffer[0] = timeCounter / 10; // Tens digit for horizontal
            led_buffer[1] = timeCounter % 10; // Ones digit for horizontal
            led_buffer[2] = (redDuration - (greenDuration + timeCounter)) > 9 ? 
                           (redDuration - (greenDuration + timeCounter)) / 10 : 0; // Tens digit for vertical
            led_buffer[3] = (redDuration - (greenDuration + timeCounter)) % 10; // Ones digit for vertical
            break;
            
        case H_RED_V_GREEN:
            led_buffer[0] = (redDuration - timeCounter) > 9 ? (redDuration - timeCounter) / 10 : 0; // Tens digit for horizontal
            led_buffer[1] = (redDuration - timeCounter) % 10; // Ones digit for horizontal
            led_buffer[2] = timeCounter / 10; // Tens digit for vertical
            led_buffer[3] = timeCounter % 10; // Ones digit for vertical
            break;
            
        case H_RED_V_YELLOW:
            led_buffer[0] = (redDuration - (greenDuration + timeCounter)) > 9 ? 
                           (redDuration - (greenDuration + timeCounter)) / 10 : 0; // Tens digit for horizontal
            led_buffer[1] = (redDuration - (greenDuration + timeCounter)) % 10; // Ones digit for horizontal
            led_buffer[2] = timeCounter / 10; // Tens digit for vertical
            led_buffer[3] = timeCounter % 10; // Ones digit for vertical
            break;
    }
}

// Update LED display buffers for configuration modes
void updateConfigModeDisplay(int mode, int value) {
    // First two digits show mode number
    led_buffer[0] = 0;
    led_buffer[1] = mode - 1; // Mode - 1 (to show 1-3 for modes 2-4)
    
    // Last two digits show the configuration value
    led_buffer[2] = value / 10;
    led_buffer[3] = value % 10;
}

// Process button presses for each button
void processButton(int index) {
    switch(buttonState[index]) {
        case BUTTON_RELEASED:
            if (is_button_pressed(index)) {
                buttonState[index] = BUTTON_PRESSED;
                
                // Process based on button index
                if (index == 0) { // Mode button
                    systemState = (systemState % 4) + 1; // Cycle through modes 1-4
                    
                    // Reset temp value for config modes
                    if (systemState >= MODE_2 && systemState <= MODE_4) {
                        switch(systemState) {
                            case MODE_2:
                                tempValue = redDuration;
                                break;
                            case MODE_3:
                                tempValue = yellowDuration;
                                break;
                            case MODE_4:
                                tempValue = greenDuration;
                                break;
                        }
                    }
                    
                    // Reset blink timer
                    if (systemState != MODE_1) {
                        setTimer2(250); // 2Hz blinking (500ms period)
                        clearAllTrafficLights();
                    }
                }
                else if (index == 1 && systemState != MODE_1) { // Increase value button (not in mode 1)
                    tempValue++;
                    if (tempValue > 99) tempValue = 1; // Wrap around
                }
                else if (index == 2 && systemState != MODE_1) { // Set value button (not in mode 1)
                    // Set the appropriate duration based on mode
                    switch(systemState) {
                        case MODE_2:
                            redDuration = tempValue;
                            break;
                        case MODE_3:
                            yellowDuration = tempValue;
                            break;
                        case MODE_4:
                            greenDuration = tempValue;
                            break;
                    }
                }
            }
            break;
            
        case BUTTON_PRESSED:
            if (!is_button_pressed(index)) {
                buttonState[index] = BUTTON_RELEASED;
            } else {
                if (is_button_pressed_1s(index)) {
                    buttonState[index] = BUTTON_PRESSED_MORE_THAN_1_SECOND;
                    
                    // Auto-increase for button 1 (value increase)
                    if (index == 1 && systemState != MODE_1) {
                        setTimer3(100); // Auto-increment every 100ms
                    }
                }
            }
            break;
            
        case BUTTON_PRESSED_MORE_THAN_1_SECOND:
            if (!is_button_pressed(index)) {
                buttonState[index] = BUTTON_RELEASED;
            } else {
                // Auto-increase for button 1 (only in config modes)
                if (index == 1 && systemState != MODE_1 && timer3_flag) {
                    tempValue++;
                    if (tempValue > 99) tempValue = 1; // Wrap around
                    setTimer3(100); // Reset auto-increment timer
                }
            }
            break;
    }
}

void fsm_for_input_processing(void) {
    // Process button inputs
    for (int i = 0; i < 3; i++) {
        processButton(i);
    }

    // Main FSM for system states
    switch(systemState) {
        case INIT:
            // Initialize system
            systemState = MODE_1;
            trafficState = H_GREEN_V_RED;
            timeCounter = 0;
            setTimer1(1000); // 1 second timer for traffic lights
            break;
            
        case MODE_1: // Normal mode
            if (timer1_flag) {
                // Update traffic light state
                switch(trafficState) {
                    case H_GREEN_V_RED:
                        timeCounter++;
                        if (timeCounter >= greenDuration) {
                            trafficState = H_YELLOW_V_RED;
                            timeCounter = 0;
                            setColorLights(1, 0); // H: Yellow, V: Red
                        }
                        break;
                        
                    case H_YELLOW_V_RED:
                        timeCounter++;
                        if (timeCounter >= yellowDuration) {
                            trafficState = H_RED_V_GREEN;
                            timeCounter = 0;
                            setColorLights(0, 2); // H: Red, V: Green
                        }
                        break;
                        
                    case H_RED_V_GREEN:
                        timeCounter++;
                        if (timeCounter >= greenDuration) {
                            trafficState = H_RED_V_YELLOW;
                            timeCounter = 0;
                            setColorLights(0, 1); // H: Red, V: Yellow
                        }
                        break;
                        
                    case H_RED_V_YELLOW:
                        timeCounter++;
                        if (timeCounter >= yellowDuration) {
                            trafficState = H_GREEN_V_RED;
                            timeCounter = 0;
                            setColorLights(2, 0); // H: Green, V: Red
                        }
                        break;
                }
                
                // Set traffic lights based on current state
                switch(trafficState) {
                    case H_GREEN_V_RED:
                        setColorLights(2, 0); // H: Green, V: Red
                        break;
                    case H_YELLOW_V_RED:
                        setColorLights(1, 0); // H: Yellow, V: Red
                        break;
                    case H_RED_V_GREEN:
                        setColorLights(0, 2); // H: Red, V: Green
                        break;
                    case H_RED_V_YELLOW:
                        setColorLights(0, 1); // H: Red, V: Yellow
                        break;
                }
                
                updateNormalModeDisplay();
                setTimer1(1000); // Reset timer
            }
            break;
            
        case MODE_2: // Red duration configuration
            updateConfigModeDisplay(2, tempValue);
            
            // Blink red LEDs at 2Hz
            if (timer2_flag) {
                blinkAllLights(0); // Blink red lights
                setTimer2(250); // 2Hz blinking (500ms period)
            }
            break;
            
        case MODE_3: // Yellow duration configuration
            updateConfigModeDisplay(3, tempValue);
            
            // Blink yellow LEDs at 2Hz
            if (timer2_flag) {
                blinkAllLights(1); // Blink yellow lights
                setTimer2(250); // 2Hz blinking (500ms period)
            }
            break;
            
        case MODE_4: // Green duration configuration
            updateConfigModeDisplay(4, tempValue);
            
            // Blink green LEDs at 2Hz
            if (timer2_flag) {
                blinkAllLights(2); // Blink green lights
                setTimer2(250); // 2Hz blinking (500ms period)
            }
            break;
    }
    
    // Update 7-segment display (cycle through digits)
    if (index_led >= MAX_LED) {
        index_led = 0;
    }
    update7SEG(index_led++);
}
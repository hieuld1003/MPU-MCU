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

// Button states for button processing
enum ButtonState {BUTTON_RELEASED, BUTTON_PRESSED, BUTTON_PRESSED_MORE_THAN_1_SECOND};
enum ButtonState buttonState[3] = {BUTTON_RELEASED, BUTTON_RELEASED, BUTTON_RELEASED};

// System mode and temporary value
int currentMode = MODE_NORMAL;
int tempValue = 0;
int isConfigUpdated = 0;

void fsm_for_input_processing(void) {
    // Xử lý trạng thái khởi tạo ban đầu
    static int isInitialized = 0;
    if (!isInitialized) {
        trafficLightInit();
        setTimer(&timerForMode1, 1000);
        isInitialized = 1;
    }
    
    // Process button 0 (Mode selection)
    switch(buttonState[0]) {
        case BUTTON_RELEASED:
            if(is_button_pressed(0)) {
                buttonState[0] = BUTTON_PRESSED;
                
                // Change mode when button is pressed
                currentMode = (currentMode % 4) + 1;
                
                // Initialize temp value based on the current mode
                switch(currentMode) {
                    case MODE_NORMAL:
                        // Reinitialize traffic lights for normal mode
                        trafficLightInit();
                        setTimer(&timerForMode1, 1000);
                        break;
                        
                    case MODE_RED:
                        tempValue = redDuration;
                        setTimer(&timerForBlink, 500); // Start blinking
                        clearBlinkingLeds();
                        break;
                        
                    case MODE_AMBER:
                        tempValue = amberDuration;
                        setTimer(&timerForBlink, 500); // Start blinking
                        clearBlinkingLeds();
                        break;
                        
                    case MODE_GREEN:
                        tempValue = greenDuration;
                        setTimer(&timerForBlink, 500); // Start blinking
                        clearBlinkingLeds();
                        break;
                }
            }
            break;
            
        case BUTTON_PRESSED:
            if(!is_button_pressed(0)) {
                buttonState[0] = BUTTON_RELEASED;
            } else if(is_button_pressed_1s(0)) {
                buttonState[0] = BUTTON_PRESSED_MORE_THAN_1_SECOND;
            }
            break;
            
        case BUTTON_PRESSED_MORE_THAN_1_SECOND:
            if(!is_button_pressed(0)) {
                buttonState[0] = BUTTON_RELEASED;
            }
            break;
    }
    
    // Process button 1 (Increment value)
    switch(buttonState[1]) {
        case BUTTON_RELEASED:
            if(is_button_pressed(1) && currentMode != MODE_NORMAL) {
                buttonState[1] = BUTTON_PRESSED;
                
                // Increment temp value
                tempValue++;
                if (tempValue > 99) tempValue = 1;
            }
            break;
            
        case BUTTON_PRESSED:
            if(!is_button_pressed(1)) {
                buttonState[1] = BUTTON_RELEASED;
            } else if(is_button_pressed_1s(1)) {
                buttonState[1] = BUTTON_PRESSED_MORE_THAN_1_SECOND;
                setTimer(&timerForAutoInc, 500); // 0.5s for auto-increment
            }
            break;
            
        case BUTTON_PRESSED_MORE_THAN_1_SECOND:
            if(!is_button_pressed(1)) {
                buttonState[1] = BUTTON_RELEASED;
            } else {
                // Auto-increment the value
                if (timerFlag(&timerForAutoInc)) {
                    tempValue++;
                    if (tempValue > 99) tempValue = 1;
                    setTimer(&timerForAutoInc, 500); // 0.5s for next auto-increment
                }
            }
            break;
    }
    
    // Process button 2 (Set value)
    switch(buttonState[2]) {
        case BUTTON_RELEASED:
            if(is_button_pressed(2) && currentMode != MODE_NORMAL) {
                buttonState[2] = BUTTON_PRESSED;
                
                // Set the value based on the current mode
                switch(currentMode) {
                    case MODE_RED:
                        redDuration = tempValue;
                        break;
                    case MODE_AMBER:
                        amberDuration = tempValue;
                        break;
                    case MODE_GREEN:
                        greenDuration = tempValue;
                        break;
                }
                isConfigUpdated = 1;
            }
            break;
            
        case BUTTON_PRESSED:
            if(!is_button_pressed(2)) {
                buttonState[2] = BUTTON_RELEASED;
            } else if(is_button_pressed_1s(2)) {
                buttonState[2] = BUTTON_PRESSED_MORE_THAN_1_SECOND;
            }
            break;
            
        case BUTTON_PRESSED_MORE_THAN_1_SECOND:
            if(!is_button_pressed(2)) {
                buttonState[2] = BUTTON_RELEASED;
            }
            break;
    }
    
    // Update displays and LEDs based on current mode
    switch(currentMode) {
        case MODE_NORMAL:
            if (timerFlag(&timerForMode1)) {
                updateTrafficLights();
                setTimer(&timerForMode1, 1000); // Update traffic lights every 1s
            }
            break;
            
        case MODE_RED:
            blinkRedLeds();
            break;
            
        case MODE_AMBER:
            blinkAmberLeds();
            break;
            
        case MODE_GREEN:
            blinkGreenLeds();
            break;
    }
    
    // Always update displays regardless of mode
    updateDisplays();
}
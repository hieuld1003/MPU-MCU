/*
 * input_processing.c
 *
 *  Created on: Oct 2, 2025
 *      Author: ADMIN
 */

#include "main.h"
#include "input_reading.h"
#include "led_display.h"

enum ButtonState {BUTTON_RELEASED, BUTTON_PRESSED, BUTTON_PRESSED_MORE_THAN_1_SECOND};

// Button states for the 3 buttons
enum ButtonState buttonState[3] = {BUTTON_RELEASED, BUTTON_RELEASED, BUTTON_RELEASED};

// Function to handle mode button (sw0)
void handleModeButton() {
    switch(buttonState[0]) {
        case BUTTON_RELEASED:
            if (is_button_pressed(0)) {
                buttonState[0] = BUTTON_PRESSED;
            }
            break;

        case BUTTON_PRESSED:
            if (!is_button_pressed(0)) {
                buttonState[0] = BUTTON_RELEASED;
                
                // Change mode on button release
                int next_mode = getMode() + 1;
                if (next_mode > 4) next_mode = 1;
                setMode(next_mode);
                
                // Clear all lights when changing modes
                clearAllLights();
            } else {
                if (is_button_pressed_1s(0)) {
                    buttonState[0] = BUTTON_PRESSED_MORE_THAN_1_SECOND;
                }
            }
            break;

        case BUTTON_PRESSED_MORE_THAN_1_SECOND:
            if (!is_button_pressed(0)) {
                buttonState[0] = BUTTON_RELEASED;
            }
            break;
    }
}

// Function to handle value adjustment button (sw1)
void handleIncreaseButton() {
    switch(buttonState[1]) {
        case BUTTON_RELEASED:
            if (is_button_pressed(1)) {
                buttonState[1] = BUTTON_PRESSED;
                
                // Increase the appropriate duration based on mode
                switch(getMode()) {
                    case 2: // Red duration
                        if (getRedDuration() < 99) {
                            setRedDuration(getRedDuration() + 1);
                        }
                        break;
                    case 3: // Yellow duration
                        if (getYellowDuration() < 99) {
                            setYellowDuration(getYellowDuration() + 1);
                        }
                        break;
                    case 4: // Green duration
                        if (getGreenDuration() < 99) {
                            setGreenDuration(getGreenDuration() + 1);
                        }
                        break;
                }
            }
            break;

        case BUTTON_PRESSED:
            if (!is_button_pressed(1)) {
                buttonState[1] = BUTTON_RELEASED;
            } else {
                if (is_button_pressed_1s(1)) {
                    buttonState[1] = BUTTON_PRESSED_MORE_THAN_1_SECOND;
                }
            }
            break;

        case BUTTON_PRESSED_MORE_THAN_1_SECOND:
            if (!is_button_pressed(1)) {
                buttonState[1] = BUTTON_RELEASED;
            } else {
                // Auto-increase value while button is held
                static int counter = 0;
                counter++;
                if (counter >= 20) { // Auto-increase every 200ms
                    counter = 0;
                    
                    // Increase the appropriate duration based on mode
                    switch(getMode()) {
                        case 2: // Red duration
                            if (getRedDuration() < 99) {
                                setRedDuration(getRedDuration() + 1);
                            }
                            break;
                        case 3: // Yellow duration
                            if (getYellowDuration() < 99) {
                                setYellowDuration(getYellowDuration() + 1);
                            }
                            break;
                        case 4: // Green duration
                            if (getGreenDuration() < 99) {
                                setGreenDuration(getGreenDuration() + 1);
                            }
                            break;
                    }
                }
            }
            break;
    }
}

// Function to handle set button (sw2)
void handleSetButton() {
    switch(buttonState[2]) {
        case BUTTON_RELEASED:
            if (is_button_pressed(2)) {
                buttonState[2] = BUTTON_PRESSED;
                
                // Set the values and return to mode 1
                setMode(1);
                clearAllLights();
            }
            break;

        case BUTTON_PRESSED:
            if (!is_button_pressed(2)) {
                buttonState[2] = BUTTON_RELEASED;
            } else {
                if (is_button_pressed_1s(2)) {
                    buttonState[2] = BUTTON_PRESSED_MORE_THAN_1_SECOND;
                }
            }
            break;

        case BUTTON_PRESSED_MORE_THAN_1_SECOND:
            if (!is_button_pressed(2)) {
                buttonState[2] = BUTTON_RELEASED;
            }
            break;
    }
}

void fsm_for_input_processing(void) {
    // Process all buttons
    handleModeButton();
    
    // In configuration modes, handle increase and set buttons
    if (getMode() > 1) {
        handleIncreaseButton();
        handleSetButton();
    }
    
    // Update displays based on current state
    updateDisplay();
}
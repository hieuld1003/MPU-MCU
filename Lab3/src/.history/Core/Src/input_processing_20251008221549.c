/*
 * input_processing.c
 *
 *  Created on: Oct 2, 2025
 *      Author: ADMIN
 */

#include "main.h"
#include "input_reading.h"
#include "led_display.h"
#include "timer.h"

// Traffic light states
#define INIT_STATE      0
#define RED_GREEN       1
#define RED_YELLOW      2
#define GREEN_RED       3
#define YELLOW_RED      4

// Operating modes
#define NORMAL_MODE     0
#define RED_MODE        1
#define YELLOW_MODE     2
#define GREEN_MODE      3

// Button indexes
#define BUTTON_MODE     0  // sw0
#define BUTTON_INCREASE 1  // sw1
#define BUTTON_SET      2  // sw2

// Default durations (in seconds)
#define DEFAULT_RED     5
#define DEFAULT_YELLOW  2
#define DEFAULT_GREEN   3

// Variables for traffic light control
int mode = NORMAL_MODE;
int traffic_state = INIT_STATE;
int counter = 0;

int red_duration = DEFAULT_RED;
int yellow_duration = DEFAULT_YELLOW;
int green_duration = DEFAULT_GREEN;

// Button states
enum ButtonState {BUTTON_RELEASED, BUTTON_PRESSED, BUTTON_PRESSED_MORE_THAN_1_SECOND};
enum ButtonState buttonState[3] = {BUTTON_RELEASED, BUTTON_RELEASED, BUTTON_RELEASED};

// Function to control traffic light LEDs
void set_traffic_lights(int horizontal_light, int vertical_light) {
    // Horizontal lights
    HAL_GPIO_WritePin(red_h_GPIO_Port, red_h_Pin, (horizontal_light == 0) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(yellow_h_GPIO_Port, yellow_h_Pin, (horizontal_light == 1) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(green_h_GPIO_Port, green_h_Pin, (horizontal_light == 2) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    
    // Vertical lights
    HAL_GPIO_WritePin(red_v_GPIO_Port, red_v_Pin, (vertical_light == 0) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(yellow_v_GPIO_Port, yellow_v_Pin, (vertical_light == 1) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(green_v_GPIO_Port, green_v_Pin, (vertical_light == 2) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

// Function to update display in normal mode
void update_normal_display() {
    switch (traffic_state) {
        case RED_GREEN:
            update_clock_buffer(0, 0);
            update_clock_buffer(1, red_duration - counter);
            update_clock_buffer(2, 0);
            update_clock_buffer(3, green_duration - counter);
            break;
            
        case RED_YELLOW:
            update_clock_buffer(0, 0);
            update_clock_buffer(1, red_duration - counter);
            update_clock_buffer(2, 0);
            update_clock_buffer(3, yellow_duration - counter);
            break;
            
        case GREEN_RED:
            update_clock_buffer(0, 0);
            update_clock_buffer(1, green_duration - counter);
            update_clock_buffer(2, 0);
            update_clock_buffer(3, red_duration - counter);
            break;
            
        case YELLOW_RED:
            update_clock_buffer(0, 0);
            update_clock_buffer(1, yellow_duration - counter);
            update_clock_buffer(2, 0);
            update_clock_buffer(3, red_duration - counter);
            break;
            
        default:
            break;
    }
}

// Function to update display in configuration mode
void update_config_display() {
    switch (mode) {
        case RED_MODE:
            update_clock_buffer(0, 0);
            update_clock_buffer(1, red_duration);
            update_clock_buffer(2, 0);
            update_clock_buffer(3, 1); // Display 1 to indicate RED config mode
            break;
            
        case YELLOW_MODE:
            update_clock_buffer(0, 0);
            update_clock_buffer(1, yellow_duration);
            update_clock_buffer(2, 0);
            update_clock_buffer(3, 2); // Display 2 to indicate YELLOW config mode
            break;
            
        case GREEN_MODE:
            update_clock_buffer(0, 0);
            update_clock_buffer(1, green_duration);
            update_clock_buffer(2, 0);
            update_clock_buffer(3, 3); // Display 3 to indicate GREEN config mode
            break;
    }
}

// Traffic light state machine
void fsm_traffic_light() {
    if (mode != NORMAL_MODE) return;
    
    switch (traffic_state) {
        case INIT_STATE:
            traffic_state = RED_GREEN;
            counter = 0;
            set_traffic_lights(0, 2); // Horizontal: RED, Vertical: GREEN
            setTimer(1000); // 1 second timer
            break;
            
        case RED_GREEN:
            if (timer_flag == 1) {
                counter++;
                if (counter >= green_duration) {
                    traffic_state = RED_YELLOW;
                    counter = 0;
                    set_traffic_lights(0, 1); // Horizontal: RED, Vertical: YELLOW
                }
                setTimer(1000);
            }
            break;
            
        case RED_YELLOW:
            if (timer_flag == 1) {
                counter++;
                if (counter >= yellow_duration) {
                    traffic_state = GREEN_RED;
                    counter = 0;
                    set_traffic_lights(2, 0); // Horizontal: GREEN, Vertical: RED
                }
                setTimer(1000);
            }
            break;
            
        case GREEN_RED:
            if (timer_flag == 1) {
                counter++;
                if (counter >= green_duration) {
                    traffic_state = YELLOW_RED;
                    counter = 0;
                    set_traffic_lights(1, 0); // Horizontal: YELLOW, Vertical: RED
                }
                setTimer(1000);
            }
            break;
            
        case YELLOW_RED:
            if (timer_flag == 1) {
                counter++;
                if (counter >= yellow_duration) {
                    traffic_state = RED_GREEN;
                    counter = 0;
                    set_traffic_lights(0, 2); // Horizontal: RED, Vertical: GREEN
                }
                setTimer(1000);
            }
            break;
    }
}

void fsm_for_input_processing(void) {
    // Run traffic light state machine in normal mode
    if (mode == NORMAL_MODE) {
        fsm_traffic_light();
        update_normal_display();
    } else {
        update_config_display();
    }
    
    // Process MODE button (sw0)
    switch (buttonState[BUTTON_MODE]) {
        case BUTTON_RELEASED:
            if (is_button_pressed(BUTTON_MODE)) {
                buttonState[BUTTON_MODE] = BUTTON_PRESSED;
                // Toggle between modes
                mode = (mode + 1) % 4;
                
                if (mode == NORMAL_MODE) {
                    // Return to normal operation
                    traffic_state = INIT_STATE;
                }
            }
            break;
            
        case BUTTON_PRESSED:
            if (!is_button_pressed(BUTTON_MODE)) {
                buttonState[BUTTON_MODE] = BUTTON_RELEASED;
            } else if (is_button_pressed_1s(BUTTON_MODE)) {
                buttonState[BUTTON_MODE] = BUTTON_PRESSED_MORE_THAN_1_SECOND;
            }
            break;
            
        case BUTTON_PRESSED_MORE_THAN_1_SECOND:
            if (!is_button_pressed(BUTTON_MODE)) {
                buttonState[BUTTON_MODE] = BUTTON_RELEASED;
            }
            break;
    }
    
    // Process INCREASE button (sw1)
    switch (buttonState[BUTTON_INCREASE]) {
        case BUTTON_RELEASED:
            if (is_button_pressed(BUTTON_INCREASE)) {
                buttonState[BUTTON_INCREASE] = BUTTON_PRESSED;
                
                // Increase timing value in configuration mode
                switch (mode) {
                    case RED_MODE:
                        red_duration = (red_duration >= 99) ? 1 : red_duration + 1;
                        break;
                        
                    case YELLOW_MODE:
                        yellow_duration = (yellow_duration >= 99) ? 1 : yellow_duration + 1;
                        break;
                        
                    case GREEN_MODE:
                        green_duration = (green_duration >= 99) ? 1 : green_duration + 1;
                        break;
                }
                
                update_config_display();
            }
            break;
            
        case BUTTON_PRESSED:
            if (!is_button_pressed(BUTTON_INCREASE)) {
                buttonState[BUTTON_INCREASE] = BUTTON_RELEASED;
            } else if (is_button_pressed_1s(BUTTON_INCREASE)) {
                buttonState[BUTTON_INCREASE] = BUTTON_PRESSED_MORE_THAN_1_SECOND;
            }
            break;
            
        case BUTTON_PRESSED_MORE_THAN_1_SECOND:
            if (!is_button_pressed(BUTTON_INCREASE)) {
                buttonState[BUTTON_INCREASE] = BUTTON_RELEASED;
            }
            // Auto-increase for long press
            switch (mode) {
                case RED_MODE:
                    red_duration = (red_duration >= 99) ? 1 : red_duration + 1;
                    break;
                    
                case YELLOW_MODE:
                    yellow_duration = (yellow_duration >= 99) ? 1 : yellow_duration + 1;
                    break;
                    
                case GREEN_MODE:
                    green_duration = (green_duration >= 99) ? 1 : green_duration + 1;
                    break;
            }
            
            update_config_display();
            break;
    }
    
    // Process SET button (sw2)
    switch (buttonState[BUTTON_SET]) {
        case BUTTON_RELEASED:
            if (is_button_pressed(BUTTON_SET)) {
                buttonState[BUTTON_SET] = BUTTON_PRESSED;
                // Set button can be used for confirming configuration changes
                // or additional functions like toggling between decimal and hex display
            }
            break;
            
        case BUTTON_PRESSED:
            if (!is_button_pressed(BUTTON_SET)) {
                buttonState[BUTTON_SET] = BUTTON_RELEASED;
            } else if (is_button_pressed_1s(BUTTON_SET)) {
                buttonState[BUTTON_SET] = BUTTON_PRESSED_MORE_THAN_1_SECOND;
            }
            break;
            
        case BUTTON_PRESSED_MORE_THAN_1_SECOND:
            if (!is_button_pressed(BUTTON_SET)) {
                buttonState[BUTTON_SET] = BUTTON_RELEASED;
            }
            break;
    }
}
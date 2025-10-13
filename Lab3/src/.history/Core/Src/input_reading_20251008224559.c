/*
 * input_reading.c
 *
 *  Created on: Oct 2, 2025
 *      Author: ADMIN
 */

#include "main.h"
#include "input_reading.h"

#define DURATION_FOR_AUTO_INCREASING 100 // 1 second with 10ms tick

static GPIO_PinState buttonBuffer[NO_OF_BUTTONS];
static GPIO_PinState debounceButtonBuffer1[NO_OF_BUTTONS];
static GPIO_PinState debounceButtonBuffer2[NO_OF_BUTTONS];
static uint8_t flagForButtonPress1s[NO_OF_BUTTONS];
static uint16_t counterForButtonPress1s[NO_OF_BUTTONS];

// Map button indices to GPIO pins
GPIO_TypeDef* button_ports[NO_OF_BUTTONS] = {sw0_GPIO_Port, sw1_GPIO_Port, sw2_GPIO_Port};
uint16_t button_pins[NO_OF_BUTTONS] = {sw0_Pin, sw1_Pin, sw2_Pin};

void button_reading(void) {
    for(int i = 0; i < NO_OF_BUTTONS; i++) {
        debounceButtonBuffer2[i] = debounceButtonBuffer1[i];
        debounceButtonBuffer1[i] = HAL_GPIO_ReadPin(button_ports[i], button_pins[i]);
        
        if(debounceButtonBuffer1[i] == debounceButtonBuffer2[i]) {
            buttonBuffer[i] = debounceButtonBuffer1[i];
            
            if(buttonBuffer[i] == BUTTON_IS_PRESSED) {
                if(counterForButtonPress1s[i] < DURATION_FOR_AUTO_INCREASING) {
                    counterForButtonPress1s[i]++;
                } else {
                    flagForButtonPress1s[i] = 1;
                }
            } else {
                counterForButtonPress1s[i] = 0;
                flagForButtonPress1s[i] = 0;
            }
        }
    }
}

unsigned char is_button_pressed(unsigned char index) {
    if(index >= NO_OF_BUTTONS) return 0;
    return (buttonBuffer[index] == BUTTON_IS_PRESSED);
}

unsigned char is_button_pressed_1s(unsigned char index) {
    if(index >= NO_OF_BUTTONS) return 0;
    return (flagForButtonPress1s[index] == 1);
}
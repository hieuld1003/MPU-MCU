/*
 * input_reading.c
 *
 *  Created on: Oct 2, 2025
 *      Author: ADMIN
 */

#include "main.h"
//we aim to work with more than one buttons
#define NO_OF_BUTTONS 1
//timer interrupt duration is 10ms, so to pass 1 second,
//we need to jump to the interrupt service routine 100 time
#define DURATION_FOR_AUTO_INCREASING 100
#define BUTTON_IS_PRESSED GPIO_PIN_RESET
#define BUTTON_IS_RELEASED GPIO_PIN_SET

//the debouncing that the final result is stored after
static GPIO_PinState buttonBuffer[NO_OF_BUTTONS];
//we define two buffers for debouncing
static GPIO_PinState debounceButtonBuffer1[NO_OF_BUTTONS];
static GPIO_PinState debounceButtonBuffer2[NO_OF_BUTTONS];

//we define a flag for a button pressed more than 1 second.
static uint8_t flagForButtonPressis[NO_OF_BUTTONS];
//we define counter for automatically increasing the value
//after the button is pressed more than 1 second
static uint16_t counterForButtonPressis[NO_OF_BUTTONS];

void button_reading(void) {
    for (char i = 0; i < NO_OF_BUTTONS; i++) {
        debounceButtonBuffer2[i] = debounceButtonBuffer1[i];
        switch(i) {
            case 0:
                debounceButtonBuffer1[i] = HAL_GPIO_ReadPin(sw0_GPIO_Port, sw0_Pin);
                break;
            case 1:
                debounceButtonBuffer1[i] = HAL_GPIO_ReadPin(sw1_GPIO_Port, sw1_Pin);
                break;
            case 2:
                debounceButtonBuffer1[i] = HAL_GPIO_ReadPin(sw2_GPIO_Port, sw2_Pin);
                break;
            default:
                break;
        }
        
        if (debounceButtonBuffer1[i] == debounceButtonBuffer2[i]) {
            buttonBuffer[i] = debounceButtonBuffer1[i];
            
            if (buttonBuffer[i] == BUTTON_IS_PRESSED) {
                // If a button is pressed, we start counting
                if (counterForButtonPressis[i] < DURATION_FOR_AUTO_INCREASING) {
                    counterForButtonPressis[i]++;
                } else {
                    // The flag is turned on when 1 second has passed
                    // since the button is pressed.
                    flagForButtonPressis[i] = 1;
                }
            } else {
                // Button is released
                counterForButtonPressis[i] = 0;
                flagForButtonPressis[i] = 0;
            }
        }
    }
}
//Checking a button is pressed or not
unsigned char is_button_pressed(uint8_t index) {
	if (index >= N0_OF_BUTTONS) return 0;
	return (buttonBuffer[index] == BUTTON_IS_PRESSED);
}

//Checking a button is pressed more than a second or not
unsigned char is_button_pressed(unsigned char index) {
	if (index >= N0_OF_BUTTONS) return 0xff;
	return (flagForButtonPress1s[index] == 1);
}


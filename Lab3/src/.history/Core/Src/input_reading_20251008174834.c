/*
 * input_reading.c
 *
 *  Created on: Oct 2, 2025
 *      Author: ADMIN
 */

#include "main.h"
#include "input_reading.h"

#define NO_OF_BUTTONS 3
#define DURATION_FOR_AUTO_INCREASING 100
#define BUTTON_IS_PRESSED BTN_PRESSED
#define BUTTON_IS_RELEASED BTN_RELEASED

static int buttonBuffer[NO_OF_BUTTONS] = {BTN_RELEASED, BTN_RELEASED, BTN_RELEASED};
static int debounceButtonBuffer1[NO_OF_BUTTONS] = {BTN_RELEASED, BTN_RELEASED, BTN_RELEASED};
static int debounceButtonBuffer2[NO_OF_BUTTONS] = {BTN_RELEASED, BTN_RELEASED, BTN_RELEASED};

static int flagForButtonPress[NO_OF_BUTTONS] = {0, 0, 0};
static int counterForButtonPress[NO_OF_BUTTONS] = {0, 0, 0};
static int flagForButtonPress1s[NO_OF_BUTTONS] = {0, 0, 0};
static int counterForButtonPress1s[NO_OF_BUTTONS] = {0, 0, 0};

GPIO_TypeDef* buttonPort[NO_OF_BUTTONS] = {sw0_GPIO_Port, sw1_GPIO_Port, sw2_GPIO_Port};
uint16_t buttonPin[NO_OF_BUTTONS] = {sw0_Pin, sw1_Pin, sw2_Pin};

void button_reading(void) {
    for (int i = 0; i < NO_OF_BUTTONS; i++) {
        debounceButtonBuffer2[i] = debounceButtonBuffer1[i];
        debounceButtonBuffer1[i] = (HAL_GPIO_ReadPin(buttonPort[i], buttonPin[i]) == GPIO_PIN_RESET) ? 
                                    BTN_PRESSED : BTN_RELEASED;
        
        if (debounceButtonBuffer1[i] == debounceButtonBuffer2[i]) {
            if (buttonBuffer[i] != debounceButtonBuffer1[i]) {
                buttonBuffer[i] = debounceButtonBuffer1[i];
                
                if (buttonBuffer[i] == BUTTON_IS_PRESSED) {
                    flagForButtonPress[i] = 1;
                }
            } else {
                counterForButtonPress1s[i]++;
                if (counterForButtonPress1s[i] >= DURATION_FOR_AUTO_INCREASING) {
                    flagForButtonPress1s[i] = 1;
                }
            }
        } else {
            counterForButtonPress1s[i] = 0;
        }
        
        if (buttonBuffer[i] == BUTTON_IS_RELEASED) {
            flagForButtonPress1s[i] = 0;
            counterForButtonPress1s[i] = 0;
        }
    }
}

unsigned char is_button_pressed(unsigned char index) {
    if (index >= NO_OF_BUTTONS) return 0;
    
    if (flagForButtonPress[index]) {
        flagForButtonPress[index] = 0;
        return 1;
    }
    return 0;
}

unsigned char is_button_pressed_1s(unsigned char index) {
    if (index >= NO_OF_BUTTONS) return 0;
    
    if (flagForButtonPress1s[index]) {
        return 1;
    }
    return 0;
}
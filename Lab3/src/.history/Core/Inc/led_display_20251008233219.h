/*
 * led_display.h
 *
 *  Created on: Oct 2, 2025
 *      Author: ADMIN
 */

#ifndef INC_LED_DISPLAY_H_
#define INC_LED_DISPLAY_H_

// Traffic light state definitions
#define RED 0
#define AMBER 1
#define GREEN 2

// Display segment definitions
extern int arr[10][7];

// Traffic light control functions
void trafficLightInit(void);
void updateTrafficLights(void);
void blinkRedLeds(void);
void blinkAmberLeds(void);
void blinkGreenLeds(void);
void clearBlinkingLeds(void);

// 7-segment display functions
void display7SEG_h(int num);
void display7SEG_v(int num);
void updateDisplays(void);
void turnOffAllDisplay(void);
void selectDisplay(int index);

#endif /* INC_LED_DISPLAY_H_ */

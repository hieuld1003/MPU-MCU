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
extern int segmentData[10];

// Traffic light control functions
void trafficLightInit(void);
void updateTrafficLights(void);
void blinkRedLeds(void);
void blinkAmberLeds(void);
void blinkGreenLeds(void);
void clearBlinkingLeds(void);

// 7-segment display functions
void displaySegment(int num, int isVertical);
void updateDisplays(void);
void turnOffAllDisplay(void);
void selectDisplay(int index);

#endif /* INC_LED_DISPLAY_H_ */

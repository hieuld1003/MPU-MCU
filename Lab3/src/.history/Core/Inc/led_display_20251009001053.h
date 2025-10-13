/*
 * led_display.h
 *
 *  Created on: Oct 2, 2025
 *      Author: ADMIN
 */

#ifndef INC_LED_DISPLAY_H_
#define INC_LED_DISPLAY_H_

// LED states
#define RED     0
#define AMBER   1
#define GREEN   2

// Global variables
extern int displayBuffer[4];

// Function prototypes
void displayInit(void);
void scan7Seg(void);
void displayLED7Seg(int num, int isVertical);
void setTrafficLight(int horizontal_state, int vertical_state);
void updateClockBuffer(void);
void blinkAllRed(void);
void blinkAllYellow(void);
void blinkAllGreen(void);
void clearAllLights(void);

#endif /* INC_LED_DISPLAY_H_ */
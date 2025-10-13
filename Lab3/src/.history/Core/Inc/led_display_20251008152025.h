/*
 * led_display.h
 *
 *  Created on: Oct 2, 2025
 *      Author: ADMIN
 */

#ifndef INC_LED_DISPLAY_H_
#define INC_LED_DISPLAY_H_

// Traffic light states
enum TrafficLightMode {
    NORMAL_MODE = 0,
    RED_CONFIG_MODE,
    AMBER_CONFIG_MODE, 
    GREEN_CONFIG_MODE
};

// Function prototypes
void display7SEG(int num, int horizontalDisplay);
void update7SEG(int index);
void updateLEDBuffer(int mode, int value);
void displayTrafficLight(enum TrafficLightMode mode, int timerFlag);
void led_init(void);

// Set the mode and update displays
void setCurrentMode(enum TrafficLightMode mode);
enum TrafficLightMode getCurrentMode(void);

// For timing and blinking functions
void updateLEDs(void);


#endif /* INC_LED_DISPLAY_H_ */

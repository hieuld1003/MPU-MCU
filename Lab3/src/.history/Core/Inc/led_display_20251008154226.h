/*
 * led_display.h
 *
 *  Created on: Oct 2, 2025
 *      Author: ADMIN
 */

#ifndef INC_LED_DISPLAY_H_
#define INC_LED_DISPLAY_H_

// 7-segment display functions
void display7SEG_H(int num);
void display7SEG_V(int num);
void enable_7seg(int index);
void update7SEG();

// Traffic light display functions
void updateLEDBuffer(int time_value, int mode);
void clearAllLights();
void toggleModeLEDs();
void displayTrafficLight();

// Main update function
void updateDisplay();

// Getters and setters for traffic light durations
void setRedDuration(int duration);
void setYellowDuration(int duration);
void setGreenDuration(int duration);
int getRedDuration();
int getYellowDuration();
int getGreenDuration();

// Mode handling
void setMode(int mode);
int getMode();

// External variables
extern int current_mode;
extern int red_duration;
extern int yellow_duration;
extern int green_duration;

#endif /* INC_LED_DISPLAY_H_ */

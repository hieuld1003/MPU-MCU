/*
 * timer.h
 *
 *  Created on: Oct 2, 2025
 *      Author: ADMIN
 */

#ifndef INC_TIMER_H_
#define INC_TIMER_H_

#define TICK_TIME 10 // 10ms tick

// Timer definitions for different features
extern int timerForBlink;
extern int timerForMode1;
extern int timerForAutoInc;

// Traffic light durations (in seconds)
extern int redDuration;
extern int amberDuration;
extern int greenDuration;

// Traffic light counters
extern int horizontalCounter;
extern int verticalCounter;

// Timer functions
void initTimers(void);
void timerRun(void);
void setTimer(int *timer, int duration);
int timerFlag(int *timer);
void updateTimers(void);

#endif /* INC_TIMER_H_ */

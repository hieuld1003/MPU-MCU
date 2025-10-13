/*
 * timer.h
 *
 *  Created on: Oct 2, 2025
 *      Author: ADMIN
 */

#ifndef INC_TIMER_H_
#define INC_TIMER_H_

#define TICK_TIME 10 // 10ms tick

// Timer flags
extern int timer1_flag;
extern int timer2_flag;
extern int timer3_flag;

// Traffic light durations (in seconds)
extern int redDuration;
extern int amberDuration;
extern int greenDuration;

// Traffic light counters
extern int horizontalCounter;
extern int verticalCounter;

// Timer functions
void setTimer1(int duration);
void setTimer2(int duration);
void setTimer3(int duration);
void timerRun(void);

#endif /* INC_TIMER_H_ */

/*
 * timer.h
 *
 *  Created on: Oct 2, 2025
 *      Author: ADMIN
 */

#ifndef INC_TIMER_H_
#define INC_TIMER_H_

extern int timer_flag;
extern int timer_counter;

void setTimer(int duration);
void timerRun(void);

#endif /* INC_TIMER_H_ */

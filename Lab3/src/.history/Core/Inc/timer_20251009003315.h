/*
 * timer.h
 *
 *  Created on: Oct 2, 2025
 *      Author: ADMIN
 */

#ifndef INC_TIMER_H_
#define INC_TIMER_H_

#include "main.h"

extern int timer_traffic_light_flag;
extern int timer_7seg_scan_flag;

void setTimer_TrafficLight(int duration);
void setTimer_7seg_scan(int duration);
void timer_run();

#endif /* INC_TIMER_H_ */
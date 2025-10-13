/*
 * input_processing.h
 *
 *  Created on: Oct 2, 2025
 *      Author: ADMIN
 */

#ifndef INC_INPUT_PROCESSING_H_
#define INC_INPUT_PROCESSING_H_

#include "main.h"

// System states
#define INIT 0
#define MODE_1 1
#define MODE_2 2
#define MODE_3 3
#define MODE_4 4

// Traffic light states
#define H_GREEN_V_RED 10
#define H_YELLOW_V_RED 11
#define H_RED_V_GREEN 12
#define H_RED_V_YELLOW 13

extern int systemState;
extern int trafficState;
extern int timeCounter;
extern int tempValue;

void fsm_for_input_processing(void);

#endif /* INC_INPUT_PROCESSING_H_ */

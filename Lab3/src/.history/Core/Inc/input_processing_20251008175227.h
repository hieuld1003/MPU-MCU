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
#define NORMAL_MODE 1
#define RED_CONFIG 2
#define YELLOW_CONFIG 3
#define GREEN_CONFIG 4
#define IDLE -1

// Traffic states
#define HOR_GREEN_VER_RED 10
#define HOR_YELLOW_VER_RED 11
#define HOR_RED_VER_GREEN 12
#define HOR_RED_VER_YELLOW 13

extern int systemMode;
extern int trafficState;
extern int horCounter;
extern int verCounter;
extern int tempValue;
extern int scan;

void fsm_for_input_processing(void);

#endif /* INC_INPUT_PROCESSING_H_ */

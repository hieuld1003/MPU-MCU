/*
 * input_processing.h
 *
 *  Created on: Oct 2, 2025
 *      Author: ADMIN
 */

#ifndef INC_INPUT_PROCESSING_H_
#define INC_INPUT_PROCESSING_H_

#include "main.h"

#define NORMAL_MODE 1
#define MODIFY_RED_MODE 2
#define MODIFY_YELLOW_MODE 3
#define MODIFY_GREEN_MODE 4

#define RED_H_GREEN_V   0
#define RED_H_YELLOW_V  1
#define GREEN_H_RED_V   2
#define YELLOW_H_RED_V  3

void fsm_for_input_processing(void);

#endif /* INC_INPUT_PROCESSING_H_ */
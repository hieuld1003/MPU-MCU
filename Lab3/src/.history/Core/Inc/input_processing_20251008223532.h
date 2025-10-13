/*
 * input_processing.h
 *
 *  Created on: Oct 2, 2025
 *      Author: ADMIN
 */

#ifndef INC_INPUT_PROCESSING_H_
#define INC_INPUT_PROCESSING_H_

#define MODE_NORMAL 1
#define MODE_RED 2
#define MODE_AMBER 3
#define MODE_GREEN 4

extern int currentMode;
extern int tempValue;

void fsm_for_input_processing(void);

#endif /* INC_INPUT_PROCESSING_H_ */

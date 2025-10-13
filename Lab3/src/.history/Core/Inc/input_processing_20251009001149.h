/*
 * input_processing.h
 *
 *  Created on: Oct 2, 2025
 *      Author: ADMIN
 */

#ifndef INC_INPUT_PROCESSING_H_
#define INC_INPUT_PROCESSING_H_

// System states
#define INIT          0
#define NORMAL_MODE   1
#define RED_MODE      2
#define AMBER_MODE    3
#define GREEN_MODE    4

// Global variables
extern int status;
extern int configValue;

// Function prototypes
void fsm_for_input_processing(void);
void fsm_automatic(void);
void initialize(void);
void processButtonForMode(void);
void processButtonForInc(void);
void processButtonForSet(void);

#endif /* INC_INPUT_PROCESSING_H_ */

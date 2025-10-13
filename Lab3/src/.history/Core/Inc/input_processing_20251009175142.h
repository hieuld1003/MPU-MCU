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

// Trạng thái cho đèn chiều ngang (_h)
#define H_GREEN   10
#define H_YELLOW  11
#define H_RED     12

// Trạng thái cho đèn chiều dọc (_v)
#define V_GREEN   20
#define V_YELLOW  21
#define V_RED     22

void fsm_for_input_processing(void);

#endif /* INC_INPUT_PROCESSING_H_ */
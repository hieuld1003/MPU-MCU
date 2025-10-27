/*
 * sch.h
 *
 *  Created on: Oct 24, 2025
 *      Author: ADMIN
 */

#ifndef INC_SCH_H_
#define INC_SCH_H_

#include <stdint.h>

#define SCH_MAX_TASKS 40
#define NO_TASK 0xFF

typedef struct {
    void (*pTask)(void);
    uint32_t Period;
    uint8_t RunMe;
    uint32_t TaskID;
    uint32_t Next_Run_Time;
    uint8_t Next_Index;
} sTask;

void SCH_Init(void);
void SCH_Update(void);
void SCH_Insert_Task_Sorted(uint8_t task_index);
void SCH_Dispatch_Tasks(void);
uint32_t SCH_Add_Task(void (*pFunction)(), uint32_t DELAY, uint32_t PERIOD);
uint8_t SCH_Delete_Task(uint32_t taskID);
uint32_t SCH_Get_Time(void);

#endif /* INC_SCH_H_ */

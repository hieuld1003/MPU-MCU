/*
 * sch.c
 *
 *  Created on: Oct 24, 2025
 *      Author: ADMIN
 */

#include "sch.h"

static sTask SCH_tasks_G[SCH_MAX_TASKS];
static uint32_t current_time_ticks = 0;
static uint32_t next_task_id = 0;
static uint8_t head_index = NO_TASK;

void SCH_Init(void) {
    uint8_t i;
    for (i = 0; i < SCH_MAX_TASKS; i++) {
        SCH_tasks_G[i].pTask = 0;
        SCH_tasks_G[i].Next_Run_Time = 0;
        SCH_tasks_G[i].Period = 0;
        SCH_tasks_G[i].RunMe = 0;
        SCH_tasks_G[i].TaskID = 0;
        SCH_tasks_G[i].Next_Index = NO_TASK;
    }
    current_time_ticks = 0;
    next_task_id = 1;
    head_index = NO_TASK;
}

void SCH_Update(void) {
    current_time_ticks++;

    while (head_index != NO_TASK && 
           current_time_ticks >= SCH_tasks_G[head_index].Next_Run_Time) {
        
        SCH_tasks_G[head_index].RunMe += 1;
        
        if (SCH_tasks_G[head_index].Period > 0) {
            SCH_tasks_G[head_index].Next_Run_Time = 
                current_time_ticks + SCH_tasks_G[head_index].Period;
            
            uint8_t current_task_index = head_index;
            head_index = SCH_tasks_G[head_index].Next_Index;
            
            SCH_Insert_Task_Sorted(current_task_index);
        } else {
            head_index = SCH_tasks_G[head_index].Next_Index;
        }
    }
}

void SCH_Insert_Task_Sorted(uint8_t task_index) {
    if (head_index == NO_TASK || 
        SCH_tasks_G[task_index].Next_Run_Time < SCH_tasks_G[head_index].Next_Run_Time) {
        SCH_tasks_G[task_index].Next_Index = head_index;
        head_index = task_index;
        return;
    }
    
    uint8_t current = head_index;
    while (SCH_tasks_G[current].Next_Index != NO_TASK) {
        uint8_t next = SCH_tasks_G[current].Next_Index;
        if (SCH_tasks_G[task_index].Next_Run_Time < SCH_tasks_G[next].Next_Run_Time) {
            SCH_tasks_G[task_index].Next_Index = next;
            SCH_tasks_G[current].Next_Index = task_index;
            return;
        }
        current = next;
    }
    
    SCH_tasks_G[current].Next_Index = task_index;
    SCH_tasks_G[task_index].Next_Index = NO_TASK;
}


void SCH_Dispatch_Tasks(void) {
    uint8_t Index;
    
    for (Index = 0; Index < SCH_MAX_TASKS; Index++) {
        if (SCH_tasks_G[Index].pTask && SCH_tasks_G[Index].RunMe > 0) {
            SCH_tasks_G[Index].RunMe -= 1;
            (*SCH_tasks_G[Index].pTask)();
            
            if (SCH_tasks_G[Index].Period == 0) {
                SCH_Delete_Task(SCH_tasks_G[Index].TaskID);
            }
        }
    }
}

uint32_t SCH_Add_Task(void (*pFunction)(), uint32_t DELAY, uint32_t PERIOD) {
    uint8_t Index = 0;

    while ((SCH_tasks_G[Index].pTask != 0) && (Index < SCH_MAX_TASKS)) {
        Index++;
    }
    
    if (Index == SCH_MAX_TASKS) {
        return SCH_MAX_TASKS;
    }

    SCH_tasks_G[Index].pTask = pFunction;
    SCH_tasks_G[Index].Next_Run_Time = current_time_ticks + DELAY;
    SCH_tasks_G[Index].Period = PERIOD;
    SCH_tasks_G[Index].RunMe = 0;
    SCH_tasks_G[Index].TaskID = next_task_id;
    SCH_tasks_G[Index].Next_Index = NO_TASK;
    
    SCH_Insert_Task_Sorted(Index);
    
    return next_task_id++;
}

uint8_t SCH_Delete_Task(uint32_t taskID) {
    uint8_t Index;

    for (Index = 0; Index < SCH_MAX_TASKS; Index++) {
        if (SCH_tasks_G[Index].TaskID == taskID) {
            if (head_index == Index) {
                head_index = SCH_tasks_G[Index].Next_Index;
            } else {
                uint8_t current = head_index;
                while (current != NO_TASK && SCH_tasks_G[current].Next_Index != Index) {
                    current = SCH_tasks_G[current].Next_Index;
                }
                if (current != NO_TASK) {
                    SCH_tasks_G[current].Next_Index = SCH_tasks_G[Index].Next_Index;
                }
            }
    
            SCH_tasks_G[Index].pTask = 0;
            SCH_tasks_G[Index].Next_Run_Time = 0;
            SCH_tasks_G[Index].Period = 0;
            SCH_tasks_G[Index].RunMe = 0;
            SCH_tasks_G[Index].TaskID = 0;
            SCH_tasks_G[Index].Next_Index = NO_TASK;
            return 1;
        }
    }
    return 0;
}

uint32_t SCH_Get_Time(void) {
    return current_time_ticks * 10;
}

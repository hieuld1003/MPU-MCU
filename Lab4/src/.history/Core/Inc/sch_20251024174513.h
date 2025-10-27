/*
 * sch.h
 *
 *  Created on: Oct 24, 2025
 *      Author: ADMIN
 */

#ifndef INC_SCH_H_
#define INC_SCH_H_

#include <stdint.h>

// Định nghĩa số lượng tác vụ tối đa
#define SCH_MAX_TASKS 40

// Cấu trúc dữ liệu tác vụ
typedef struct {
    void (*pTask)(void);    // Con trỏ hàm tới tác vụ
    uint32_t Delay;         // Độ trễ đến lần chạy tiếp theo (ticks)
    uint32_t Period;        // Chu kỳ lặp lại (ticks), 0 = one-shot
    uint8_t RunMe;          // Cờ báo tác vụ cần chạy
    uint32_t TaskID;        // ID của tác vụ
} sTask;

// Khai báo các hàm của scheduler
void SCH_Init(void);
void SCH_Update(void);
void SCH_Dispatch_Tasks(void);
uint32_t SCH_Add_Task(void (*pFunction)(), uint32_t DELAY, uint32_t PERIOD);
uint8_t SCH_Delete_Task(uint32_t taskID);
uint32_t SCH_Get_Time(void);

#endif /* INC_SCH_H_ */
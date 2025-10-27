/*
 * sch.c
 *
 *  Created on: Oct 24, 2025
 *      Author: ADMIN
 */

#include "sch.h"

// Mảng chứa các tác vụ
static sTask SCH_tasks_G[SCH_MAX_TASKS];
static uint32_t current_time_ticks = 0;
static uint32_t next_task_id = 0;

/**
 * @brief Khởi tạo scheduler
 */
void SCH_Init(void) {
    uint8_t i;
    for (i = 0; i < SCH_MAX_TASKS; i++) {
        SCH_tasks_G[i].pTask = 0;
        SCH_tasks_G[i].Delay = 0;
        SCH_tasks_G[i].Period = 0;
        SCH_tasks_G[i].RunMe = 0;
        SCH_tasks_G[i].TaskID = 0;
    }
    current_time_ticks = 0;
    next_task_id = 1;
}

/**
 * @brief Cập nhật scheduler - được gọi trong timer ISR (mỗi 10ms)
 */
void SCH_Update(void) {
    uint8_t Index;
    
    current_time_ticks++;
    
    // Duyệt qua tất cả các tác vụ
    for (Index = 0; Index < SCH_MAX_TASKS; Index++) {
        // Kiểm tra có tác vụ hợp lệ không
        if (SCH_tasks_G[Index].pTask) {
            if (SCH_tasks_G[Index].Delay == 0) {
                // Tác vụ đã đến hạn
                SCH_tasks_G[Index].RunMe += 1;
                
                if (SCH_tasks_G[Index].Period) {
                    // Tác vụ định kỳ - reset delay
                    SCH_tasks_G[Index].Delay = SCH_tasks_G[Index].Period;
                }
            } else {
                // Giảm delay
                SCH_tasks_G[Index].Delay -= 1;
            }
        }
    }
}

/**
 * @brief Điều phối và thực thi các tác vụ - được gọi trong main loop
 */
void SCH_Dispatch_Tasks(void) {
    uint8_t Index;
    
    for (Index = 0; Index < SCH_MAX_TASKS; Index++) {
        if (SCH_tasks_G[Index].RunMe > 0) {
            // Thực thi tác vụ
            (*SCH_tasks_G[Index].pTask)();
            
            // Giảm cờ RunMe
            SCH_tasks_G[Index].RunMe -= 1;
            
            // Nếu là tác vụ one-shot (Period == 0), xóa tác vụ
            if (SCH_tasks_G[Index].Period == 0) {
                SCH_Delete_Task(SCH_tasks_G[Index].TaskID);
            }
        }
    }
}

/**
 * @brief Thêm tác vụ vào scheduler
 * @param pFunction Con trỏ hàm tới tác vụ
 * @param DELAY Độ trễ ban đầu (ticks)
 * @param PERIOD Chu kỳ lặp lại (ticks), 0 = one-shot
 * @return Task ID, hoặc SCH_MAX_TASKS nếu thất bại
 */
uint32_t SCH_Add_Task(void (*pFunction)(), uint32_t DELAY, uint32_t PERIOD) {
    uint8_t Index = 0;
    
    // Tìm vị trí trống trong mảng
    while ((SCH_tasks_G[Index].pTask != 0) && (Index < SCH_MAX_TASKS)) {
        Index++;
    }
    
    // Kiểm tra có chỗ trống không
    if (Index == SCH_MAX_TASKS) {
        return SCH_MAX_TASKS; // Lỗi: không có chỗ trống
    }
    
    // Thêm tác vụ vào scheduler
    SCH_tasks_G[Index].pTask = pFunction;
    SCH_tasks_G[Index].Delay = DELAY;
    SCH_tasks_G[Index].Period = PERIOD;
    SCH_tasks_G[Index].RunMe = 0;
    SCH_tasks_G[Index].TaskID = next_task_id;
    
    return next_task_id++;
}

/**
 * @brief Xóa tác vụ khỏi scheduler
 * @param taskID ID của tác vụ cần xóa
 * @return 1 nếu thành công, 0 nếu thất bại
 */
uint8_t SCH_Delete_Task(uint32_t taskID) {
    uint8_t Index;
    
    for (Index = 0; Index < SCH_MAX_TASKS; Index++) {
        if (SCH_tasks_G[Index].TaskID == taskID) {
            SCH_tasks_G[Index].pTask = 0;
            SCH_tasks_G[Index].Delay = 0;
            SCH_tasks_G[Index].Period = 0;
            SCH_tasks_G[Index].RunMe = 0;
            SCH_tasks_G[Index].TaskID = 0;
            return 1;
        }
    }
    
    return 0; // Không tìm thấy task
}

/**
 * @brief Lấy thời gian hiện tại (ms)
 * @return Thời gian tính bằng ms
 */
uint32_t SCH_Get_Time(void) {
    return current_time_ticks * 10; // 10ms per tick
}
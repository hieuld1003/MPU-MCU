/*
 * input_processing.c
 *
 *  Created on: Oct 2, 2025
 *      Author: ADMIN
 */

#include "input_processing.h"
#include "led_display.h"
#include "timer.h"

// Thời gian mặc định cho các đèn (tính bằng giây)
int time_red = 5;
int time_yellow = 2;
int time_green = 3;

int mode = NORMAL_MODE;
int status = RED_H_GREEN_V;

int timer1_counter = 0;
int timer2_counter = 0;

void fsm_for_input_processing(void){
    switch(mode){
        case NORMAL_MODE:
            if(timer1_flag == 1){
                timer1_counter--;
                timer2_counter--;
                if(timer1_counter <= 0){
                    switch(status){
                        case RED_H_GREEN_V:
                            status = RED_H_YELLOW_V;
                            timer1_counter = time_yellow * 100;
                            break;
                        case RED_H_YELLOW_V:
                            status = GREEN_H_RED_V;
                            timer1_counter = time_green * 100;
                            timer2_counter = time_red * 100;
                            break;
                        case GREEN_H_RED_V:
                            status = YELLOW_H_RED_V;
                            timer1_counter = time_yellow * 100;
                            break;
                        case YELLOW_H_RED_V:
                            status = RED_H_GREEN_V;
                            timer1_counter = time_green * 100;
                            timer2_counter = time_red * 100;
                            break;
                    }
                }
                setTimer1(10); // 10ms
            }

            // Cập nhật buffer cho LED 7 đoạn
            switch(status){
                case RED_H_GREEN_V:
                    updateLedBuffer(timer2_counter/100, timer1_counter/100);
                    break;
                case RED_H_YELLOW_V:
                    updateLedBuffer(timer2_counter/100, timer1_counter/100);
                    break;
                case GREEN_H_RED_V:
                    updateLedBuffer(timer1_counter/100, timer2_counter/100);
                    break;
                case YELLOW_H_RED_V:
                    updateLedBuffer(timer1_counter/100, timer2_counter/100);
                    break;
            }
            setTrafficLight(status);
            break;
        case MODIFY_RED_MODE:
            // Logic cho Mode 2 sẽ được thêm ở đây
            break;
        case MODIFY_YELLOW_MODE:
            // Logic cho Mode 3 sẽ được thêm ở đây
            break;
        case MODIFY_GREEN_MODE:
            // Logic cho Mode 4 sẽ được thêm ở đây
            break;
    }
}
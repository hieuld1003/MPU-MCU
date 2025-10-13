/*
 * input_processing.c
 *
 *  Created on: Oct 2, 2025
 *      Author: ADMIN
 */

#include "input_processing.h"
#include "led_display.h"
#include "timer.h"
#include "input_reading.h"

int time_red = 5;
int time_yellow = 2;
int time_green = 3;

static int temp_value = 0;

static int mode = NORMAL_MODE;
static int prev_mode = NORMAL_MODE; 

static int status_h = H_RED;
static int status_v = V_GREEN;
static int counter_h = 0;
static int counter_v = 0;

static int button0_flag = 0;
static int button1_flag = 0;
static int button2_flag = 0;

static int auto_increase_counter = 0;

static int blink_status = 0;



void fsm_for_input_processing(void) {
    //Initialize only once
    static int is_initialized = 0;
    if (!is_initialized) {
        status_h = H_RED;
        counter_h = time_red;
        set_red_h(1); set_yellow_h(0); set_green_h(0);

        status_v = V_GREEN;
        counter_v = time_green;
        set_red_v(0); set_yellow_v(0); set_green_v(1);

        button0_flag = 1;
        button1_flag = 1;
        button2_flag = 1;

        setTimer_TrafficLight(1000);
        setTimer_7seg_scan(10);
        setTimer_Blink(250);
        is_initialized = 1;
    }

    scan7SEG();
    
    // Handle button sw0 cases
    if (is_button_pressed(0) && !button0_flag) {
        button0_flag = 1;
        prev_mode = mode; 
        mode = (mode % 4) + 1;
        
        switch(mode) {
            case MODIFY_RED_MODE:
                temp_value = time_red;
                break;
            case MODIFY_YELLOW_MODE:
                temp_value = time_yellow;
                break;
            case MODIFY_GREEN_MODE:
                temp_value = time_green;
                break;
        }
        
        if (prev_mode == MODIFY_GREEN_MODE && mode == NORMAL_MODE) {
            status_h = H_RED;
            counter_h = time_red;
            set_red_h(1); set_yellow_h(0); set_green_h(0);

            status_v = V_GREEN;
            counter_v = time_green;
            set_red_v(0); set_yellow_v(0); set_green_v(1);
            
            setTimer_TrafficLight(1000);
        }
    } else if (!is_button_pressed(0)) {
        button0_flag = 0;
    }
    
    // Handle button sw1 cases
    if (is_button_pressed(1)) {
        if (!button1_flag) {
            button1_flag = 1;
            
            if (mode != NORMAL_MODE) {
                temp_value++;
                if (temp_value > 99) temp_value = 1;
            }
        }
        
        if (is_button_pressed_1s(1)) {
            auto_increase_counter++;
            if (auto_increase_counter >= 2500) {
                auto_increase_counter = 0;
                if (mode != NORMAL_MODE) {
                    temp_value++;
                    if (temp_value > 99) temp_value = 1;
                }
            }
        }
    } else {
        button1_flag = 0;
        auto_increase_counter = 0; 
    }
    
    // Handle button sw2 cases
    if (is_button_pressed(2) && !button2_flag) {
        button2_flag = 1;
        
        if (mode != NORMAL_MODE) {
            switch(mode) {
                case MODIFY_RED_MODE:
                    time_red = temp_value;
                    if (time_green + time_yellow != time_red) {
                        int diff = time_red - time_yellow;
                        
                        if (diff > 1) {
                            time_green = diff;
                        } else {
                            time_green = 1;
                            time_yellow = time_red - time_green;
                        }
                    }
                    break;
                    
                case MODIFY_YELLOW_MODE:
                    time_yellow = temp_value;
                    if (time_green + time_yellow != time_red) {
                        time_green = time_red - time_yellow;
                        if (time_green < 1) {
                            time_green = 1;
                            time_red = time_green + time_yellow; 
                        }
                    }
                    break;
                    
                case MODIFY_GREEN_MODE:
                    time_green = temp_value;
                    if (time_green + time_yellow != time_red) {
                        time_yellow = time_red - time_green;
                        if (time_yellow < 1) {
                            time_yellow = 1;
                            time_red = time_green + time_yellow;
                        }
                    }
                    break;
            }
        }
    } else if (!is_button_pressed(2)) {
        button2_flag = 0;
    }
    

    // Main FSM logic
    switch(mode) {
        case NORMAL_MODE:
            updateLedBuffer(counter_h, counter_v);
        
            if(timer_traffic_light_flag == 1){
                setTimer_TrafficLight(500);
                counter_h--;
                counter_v--;
            }

            switch(status_h) {
                case H_RED:
                    if (counter_h <= 0) {
                        status_h = H_GREEN;
                        counter_h = time_green;
                        set_red_h(0); set_green_h(1);
                    }
                    break;
                case H_GREEN:
                    if (counter_h <= 0) {
                        status_h = H_YELLOW;
                        counter_h = time_yellow;
                        set_green_h(0); set_yellow_h(1);
                    }
                    break;
                case H_YELLOW:
                    if (counter_h <= 0) {
                        status_h = H_RED;
                        counter_h = time_red;
                        set_yellow_h(0); set_red_h(1);
                    }
                    break;
            }

            switch(status_v) {
                case V_RED:
                    if (counter_v <= 0) {
                        status_v = V_GREEN;
                        counter_v = time_green;
                        set_red_v(0); set_green_v(1);
                    }
                    break;
                case V_GREEN:
                    if (counter_v <= 0) {
                        status_v = V_YELLOW;
                        counter_v = time_yellow;
                        set_green_v(0); set_yellow_v(1);
                    }
                    break;
                case V_YELLOW:
                    if (counter_v <= 0) {
                        status_v = V_RED;
                        counter_v = time_red;
                        set_yellow_v(0); set_red_v(1);
                    }
                    break;
            }
            break;
            
        case MODIFY_RED_MODE:
            led_buffer[2] = 0;
            led_buffer[3] = 2;
            led_buffer[0] = temp_value / 10;
            led_buffer[1] = temp_value % 10;
            
            if (timer_blink_flag == 1) {
                setTimer_Blink(250); 
                blink_status = 1 - blink_status;
                
                set_red_h(blink_status);
                set_red_v(blink_status);
                set_yellow_h(0);
                set_yellow_v(0);
                set_green_h(0);
                set_green_v(0);
            }
            break;
            
        case MODIFY_YELLOW_MODE:
            led_buffer[2] = 0;
            led_buffer[3] = 3;
            led_buffer[0] = temp_value / 10;
            led_buffer[1] = temp_value % 10;
            
            if (timer_blink_flag == 1) {
                setTimer_Blink(250);
                blink_status = 1 - blink_status;
                
                set_red_h(0);
                set_red_v(0);
                set_yellow_h(blink_status);
                set_yellow_v(blink_status);
                set_green_h(0);
                set_green_v(0);
            }
            break;
            
        case MODIFY_GREEN_MODE:
            led_buffer[2] = 0;
            led_buffer[3] = 4;
            led_buffer[0] = temp_value / 10;
            led_buffer[1] = temp_value % 10;
            
            if (timer_blink_flag == 1) {
                setTimer_Blink(250);
                blink_status = 1 - blink_status;
                
                set_red_h(0);
                set_red_v(0);
                set_yellow_h(0);
                set_yellow_v(0);
                set_green_h(blink_status);
                set_green_v(blink_status);
            }
            break;
    }
}
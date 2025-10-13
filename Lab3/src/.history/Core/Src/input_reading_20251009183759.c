/*
 * input_reading.c
 *
 *  Created on: Oct 2, 2025
 *      Author: ADMIN
 */

#include "input_reading.h"

#define NO_OF_BUTTONS 3
#define BUTTON_IS_PRESSED GPIO_PIN_SET
#define BUTTON_IS_RELEASED GPIO_PIN_RESET

// Khai báo biến đơn giản hơn để lưu trạng thái nút
static GPIO_PinState buttonBuffer[NO_OF_BUTTONS];
static GPIO_PinState debounceButtonBuffer1[NO_OF_BUTTONS];
static GPIO_PinState debounceButtonBuffer2[NO_OF_BUTTONS];
static uint8_t flagForButtonPress1s[NO_OF_BUTTONS];
static uint16_t counterForButtonPress1s[NO_OF_BUTTONS];

// Mảng lưu trữ Port và Pin cho từng nút
static GPIO_TypeDef* button_ports[NO_OF_BUTTONS] = {sw0_GPIO_Port, sw1_GPIO_Port, sw2_GPIO_Port};
static uint16_t button_pins[NO_OF_BUTTONS] = {sw0_Pin, sw1_Pin, sw2_Pin};

void button_reading(void){
  for(int i = 0; i < NO_OF_BUTTONS; i++){
    // Đọc trạng thái nút
    GPIO_PinState currentState = HAL_GPIO_ReadPin(button_ports[i], button_pins[i]);
    
    // Cập nhật bộ đệm chống dội
    debounceButtonBuffer2[i] = debounceButtonBuffer1[i];
    debounceButtonBuffer1[i] = currentState;
    
    // Nếu trạng thái giống nhau trong 2 lần đọc, cập nhật buttonBuffer
    if(debounceButtonBuffer1[i] == debounceButtonBuffer2[i]) {
      buttonBuffer[i] = currentState;
    }
    
    // Xử lý trạng thái nút nhấn lâu
    if(buttonBuffer[i] == BUTTON_IS_PRESSED){
      if(counterForButtonPress1s[i] < DURATION_FOR_AUTO_INCREASING){
        counterForButtonPress1s[i]++;
      } else {
        flagForButtonPress1s[i] = 1;
      }
    } else {
      counterForButtonPress1s[i] = 0;
      flagForButtonPress1s[i] = 0;
    }
  }
}

// Đơn giản hóa hàm is_button_pressed để dễ dàng debug
unsigned char is_button_pressed(uint8_t index){
  if(index >= NO_OF_BUTTONS) return 0;
  return (buttonBuffer[index] == BUTTON_IS_PRESSED);
}

unsigned char is_button_pressed_1s(unsigned char index){
  if(index >= NO_OF_BUTTONS) return 0;
  return (flagForButtonPress1s[index] == 1);
}
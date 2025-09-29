/*
 * ex1-8.h
 *
 *  Created on: Sep 18, 2025
 *      Author: Luong Duc Hieu
 */

#ifndef SRC_EX1_8_H_
#define SRC_EX1_8_H_
#include "ex1-8.h"

int arr[11][7] = {
    {0, 0, 0, 0, 0, 0, 1}, //0
    {1, 0, 0, 1, 1, 1, 1}, //1
    {0, 0, 1, 0, 0, 1, 0}, //2
    {0, 0, 0, 0, 1, 1, 0}, //3
    {1, 0, 0, 1, 1, 0, 0}, //4
    {0, 1, 0, 0, 1, 0, 0}, //5
    {0, 1, 0, 0, 0, 0, 0}, //6
    {0, 0, 0, 1, 1, 1, 1}, //7
    {0, 0, 0, 0, 0, 0, 0}, //8
    {0, 0, 0, 0, 1, 0, 0}, //9
	{1, 1, 1, 1, 1, 1, 1}  //ALL LED TURN OFF
};
//	 a, b, c, d, e, f, g


void display7SEG(int num){
	if (num >= 0 && num <= 9){
		for (int state = 0; state < 7; state++){
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0 << state, arr[num][state]);
		}
	}
	else for (int state = 0; state < 7; state++){
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0 << state, arr[10][state]);
	}
}

/////ex1
//void toggle(){
//	HAL_GPIO_TogglePin(EN0_GPIO_Port, EN0_Pin);
//	HAL_GPIO_TogglePin(EN1_GPIO_Port, EN1_Pin);
//}
//
//void ex1(){
//	display7SEG(1);
//	HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, 0);
//	HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, 1);
//}
//
//int clock_1 = 50;
//void HAL_TIM_PeriodElapsedCallback (TIM_HandleTypeDef * htim) {
//	clock_1--;
//	if (clock_1 <= 0) {
//		switch (HAL_GPIO_ReadPin(EN0_GPIO_Port, EN0_Pin)) {
//			case 0: {
//				toggle();
//				display7SEG(2);
//				break;
//			}
//			case 1:{
//				toggle();
//				display7SEG(1);
//				break;
//			}
//		}
//	clock_1 = 50;
//	}
//}
//////

/////ex2
//int clock_1 = 100;
//int clock7Seg = 1;
//int stage = 0;
//
//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef * htim){
//	clock_1 --;
//	if (clock_1 <= 0) {
//		HAL_GPIO_TogglePin(DOT_GPIO_Port, DOT_Pin);
//		clock_1 = 100;
//	}
//	clock7Seg--;
//	if (clock7Seg <= 0){
//		switch (stage){
//			case 0:
//			{
//				HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, 0);
//				HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, 1);
//				HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, 1);
//				HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, 1);
//				display7SEG(1);
//				stage = 1;
//				break;
//			}
//			case 1:
//			{
//				HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, 1);
//				HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, 0);
//				HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, 1);
//				HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, 1);
//				display7SEG(2);
//				stage = 2;
//				break;
//			}
//			case 2:
//			{
//				HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, 1);
//				HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, 1);
//				HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, 0);
//				HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, 1);
//				display7SEG(3);
//				stage = 3;
//				break;
//			}
//			case 3:
//			{
//				HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, 1);
//				HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, 1);
//				HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, 1);
//				HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, 0);
//				display7SEG(0);
//				stage = 0;
//				break;
//			}
//		}
//		clock7Seg = 100;
//	}
//}
/////

/////ex3
const int MAX_LED = 4;
int index_led = 0;
int led_buffer[4] = {0, 1, 2, 3}; //change value in here

void ledNum(int idx){
	switch (idx){
		case 0:
		{
			HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, 0);
			HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, 1);
			HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, 1);
			HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, 1);
			break;
		}
		case 1:
		{
			HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, 1);
			HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, 0);
			HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, 1);
			HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, 1);
			break;
		}
		case 2:
		{
			HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, 1);
			HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, 1);
			HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, 0);
			HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, 1);
			break;
		}
		case 3:
		{
			HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, 1);
			HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, 1);
			HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, 1);
			HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, 0);
			break;
		}
		default:
		{
			HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, 0);
			HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, 0);
			HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, 0);
			HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, 0);
			break;
		}
	}
}

void update7SEG (int index){
	if (index_led >= MAX_LED) index_led = 0;
	index = index % 4;
	switch (index){
		case 0:{
			ledNum(index);
			display7SEG(led_buffer[index]);
			break;
		}
		case 1:{
			ledNum(index);
			display7SEG(led_buffer[index]);
			break;
		}
		case 2:{
			ledNum(index);
			display7SEG(led_buffer[index]);
			break;
		}
		case 3:{
			ledNum(index);
			display7SEG(led_buffer[index]);
			break;
		}
		default:
			ledNum(-1);
			break;
	}
}
//int clock7Seg = 50;
//int clockBlink = 100;
//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef * htim){
//	clock7Seg--;
//	clockBlink--;
//	if (clock7Seg <= 0) {
//		update7SEG(index_led++);
//		clock7Seg = 50;
//	}
//	if (clockBlink <= 0){
//		HAL_GPIO_TogglePin(DOT_GPIO_Port, DOT_Pin);
//		clockBlink = 100;
//	}
//}
/////

/////ex4
//change clock7Seg in ex3 to 25
/////

/////ex5 need ex3
int hour = 1,
	minute = 2,
	second = 3;
void updateClockBuffer(){
	led_buffer[0] = hour / 10;
	led_buffer[1] = hour % 10;
	led_buffer[2] = minute / 10;
	led_buffer[3] = minute % 10;
}
/////

/////ex6,7,8 need ex3,5 function
int timer0_counter = 0;
int timer0_flag = 0;
int timer1_counter = 0;
int timer1_flag = 0;
int TIMER_CYCLE = 10;
void setTimer0(int duration) {
	timer0_counter = duration / TIMER_CYCLE;
	timer0_flag = 0;
}
void setTimer1(int duration) {
	timer1_counter = duration / TIMER_CYCLE;
	timer1_flag = 0;
}
void timer0_run() {
	if (timer0_counter > 0) {
		timer0_counter--;
		if (timer0_counter == 0) timer0_flag = 1;
	}
}
void timer1_run() {
	if (timer1_counter > 0) {
		timer1_counter--;
		if (timer1_counter == 0) timer1_flag = 1;
	}
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef * htim){
	timer0_run();
	timer1_run();
}
/////

#endif /* SRC_EX1_8_H_ */

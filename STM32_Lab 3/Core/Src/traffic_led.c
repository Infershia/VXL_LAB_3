/*
 * traffic_led.c
 *
 *  Created on: Oct 27, 2025
 *      Author: Gia Minh
 */


#include "main.h"
#include "traffic_led.h"
#include "led_display.h"

#define N0_OF_LED 3

#define RED 0
#define AMBER 1
#define GREEN 2

//we define the buffer to store time_of_led
static uint8_t bufferTimerForLED[N0_OF_LED];

//we define some sub-variable counter for each led
static uint8_t counterRED1, counterAMBER1, counterGREEN1,  counterRED2, counterAMBER2, counterGREEN2;

//we define variable timer to save time each led
static uint8_t timeRED, timeAMBER, timeGREEN;

// Function to turn off all led
void LED_TRAFFIC_INIT(void){
	HAL_GPIO_WritePin(LED_RED1_GPIO_Port, LED_RED1_Pin, SET);
	HAL_GPIO_WritePin(LED_RED2_GPIO_Port, LED_RED2_Pin, SET);
	HAL_GPIO_WritePin(LED_AMBER1_GPIO_Port, LED_AMBER1_Pin, SET);
	HAL_GPIO_WritePin(LED_AMBER2_GPIO_Port, LED_AMBER2_Pin, SET);
	HAL_GPIO_WritePin(LED_GREEN1_GPIO_Port, LED_GREEN1_Pin, SET);
	HAL_GPIO_WritePin(LED_GREEN2_GPIO_Port, LED_GREEN2_Pin, SET);
}

// Function to load value are saved in buffer to counter and time of each led
// to use for display in 7SEG
void LED_TRAFFIC_LOAD_BUFFER(void){
	counterRED1 = bufferTimerForLED[RED];
	counterAMBER1 = bufferTimerForLED[AMBER];
	counterGREEN1 = bufferTimerForLED[GREEN];
	counterRED2 = bufferTimerForLED[RED];
	counterAMBER2 = bufferTimerForLED[AMBER];
	counterGREEN2 = bufferTimerForLED[GREEN];
	timeRED = bufferTimerForLED[RED];
	timeAMBER = bufferTimerForLED[AMBER];
	timeGREEN = bufferTimerForLED[GREEN];
}

// Function to store value in buffer with validation
void LED_TRAFFIC_STORE_BUFFER(uint8_t time, uint8_t index){
    switch(index) {
    case RED:
        bufferTimerForLED[RED] = time;
        break;

    case AMBER:
        // tính lại red = amber + green
        bufferTimerForLED[AMBER] = time;
        bufferTimerForLED[RED] = bufferTimerForLED[AMBER] + bufferTimerForLED[GREEN];
        break;

    case GREEN:
        //tính lại red = amber + green
        bufferTimerForLED[GREEN] = time;
        bufferTimerForLED[RED] = bufferTimerForLED[AMBER] + bufferTimerForLED[GREEN];
        break;

    default:
        break;
    }
}


// Function to setting led in vertical
void LED_VERTICAL_RUN(void) {
	//GREEN1 - ON
	if (counterGREEN2 > 0){
		update_vertical_clock_buffer(counterGREEN2);
		HAL_GPIO_WritePin(LED_RED1_GPIO_Port, LED_RED1_Pin, SET);
		HAL_GPIO_WritePin(LED_AMBER1_GPIO_Port, LED_AMBER1_Pin, SET);
		HAL_GPIO_WritePin(LED_GREEN1_GPIO_Port, LED_GREEN1_Pin, RESET);
		counterGREEN2--;

	}
	//AMBER1 - ON
	else if (counterAMBER2 > 0){
		update_vertical_clock_buffer(counterAMBER2);
		HAL_GPIO_WritePin(LED_RED1_GPIO_Port, LED_RED1_Pin, SET);
		HAL_GPIO_WritePin(LED_AMBER1_GPIO_Port, LED_AMBER1_Pin, RESET);
		HAL_GPIO_WritePin(LED_GREEN1_GPIO_Port, LED_GREEN1_Pin, SET);
		counterAMBER2--;
	}
	//RED1 - ON
	else if (counterRED2 > 0){
		update_vertical_clock_buffer(counterRED2);
		HAL_GPIO_WritePin(LED_RED1_GPIO_Port, LED_RED1_Pin, RESET);
		HAL_GPIO_WritePin(LED_AMBER1_GPIO_Port, LED_AMBER1_Pin, SET);
		HAL_GPIO_WritePin(LED_GREEN1_GPIO_Port, LED_GREEN1_Pin, SET);
		counterRED2--;
	}
	if(counterRED2 == 0) {
		counterRED2 = timeRED;
		counterAMBER2 = timeAMBER;
		counterGREEN2 = timeGREEN;
	}
}

// Function to setting led in horizontal
void LED_HORIZONTAL_RUN(void) {
    // RED2 - ON
    if (counterRED1 > 0){
        update_horizontal_clock_buffer(counterRED1);
        HAL_GPIO_WritePin(LED_RED2_GPIO_Port, LED_RED2_Pin, RESET);   // bật đèn đỏ
        HAL_GPIO_WritePin(LED_AMBER2_GPIO_Port, LED_AMBER2_Pin, SET);
        HAL_GPIO_WritePin(LED_GREEN2_GPIO_Port, LED_GREEN2_Pin, SET);
        counterRED1--;
    }
    // GREEN2 - ON
    else if (counterGREEN1 > 0){
        update_horizontal_clock_buffer(counterGREEN1);
        HAL_GPIO_WritePin(LED_RED2_GPIO_Port, LED_RED2_Pin, SET);
        HAL_GPIO_WritePin(LED_AMBER2_GPIO_Port, LED_AMBER2_Pin, SET);
        HAL_GPIO_WritePin(LED_GREEN2_GPIO_Port, LED_GREEN2_Pin, RESET); // bật đèn xanh
        counterGREEN1--;
    }
    // AMBER2 - ON
    else if (counterAMBER1 > 0){
        update_horizontal_clock_buffer(counterAMBER1);
        HAL_GPIO_WritePin(LED_RED2_GPIO_Port, LED_RED2_Pin, SET);
        HAL_GPIO_WritePin(LED_AMBER2_GPIO_Port, LED_AMBER2_Pin, RESET); // bật đèn vàng
        HAL_GPIO_WritePin(LED_GREEN2_GPIO_Port, LED_GREEN2_Pin, SET);
        counterAMBER1--;
    }

    // Khi cả 3 bộ đếm đều hết → reset để lặp lại chu kỳ
    if(counterRED1 == 0 && counterGREEN1 == 0 && counterAMBER1 == 0) {
        counterRED1 = timeRED;
        counterGREEN1 = timeGREEN;
        counterAMBER1 = timeAMBER;
    }
}

// Funtion is call when we want to display led in each mode
void LED_TRAFFIC_RUN(void) {
	LED_HORIZONTAL_RUN();
	LED_VERTICAL_RUN();
}

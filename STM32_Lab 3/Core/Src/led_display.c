/*
 * led_display.c
 *
 *  Created on: Oct 27, 2025
 *      Author: Gia Minh
 */

#include "main.h"
#include "led_display.h"

#define NUMBER_OF_7_SEG_LED 4

static uint8_t LED7Conversion[10]={
    0x3f,  // 0
    0x06,  // 1
    0x5b,  // 2
    0x4f,  // 3
    0x66,  // 4
    0x6d,  // 5
    0x7d,  // 6
    0x07,  // 7
    0x7f,  // 8
    0x67   // 9
};

// Buffer to store number after update LED7 time (digits)
static uint8_t numBuffer[NUMBER_OF_7_SEG_LED] = {0,0,0,0};

static uint8_t buffer[NUMBER_OF_7_SEG_LED] = {0,0,0,0};

// Turn off all LED7 (deactivate all digit enables)
void clearAllLED7(void){
    HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, GPIO_PIN_SET);
}

// Update all buffers when in modify time
void update_all_clock_buffer(uint8_t time1, uint8_t time2){
    numBuffer[0] = (time1 / 10) % 10;
    numBuffer[1] = time1 % 10;
    numBuffer[2] = (time2 / 10) % 10;
    numBuffer[3] = time2 % 10;
}

void update_horizontal_clock_buffer(uint8_t time) {
    numBuffer[0] = (time / 10) % 10;
    numBuffer[1] = time % 10;
}

void update_vertical_clock_buffer(uint8_t time) {
    numBuffer[2] = (time / 10) % 10;
    numBuffer[3] = time % 10;
}

void update_LED7_driver(uint8_t index){
    if(index >= NUMBER_OF_7_SEG_LED) return;

    clearAllLED7();

    buffer[index] = LED7Conversion[ numBuffer[index] ];

    displayLED7(index);

    switch (index){
        case 0:
            HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, GPIO_PIN_RESET);
            break;
        case 1:
            HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, GPIO_PIN_RESET);
            break;
        case 2:
            HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, GPIO_PIN_RESET);
            break;
        case 3:
            HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, GPIO_PIN_RESET);
            break;
    }
}

void displayLED7(uint8_t index){
    if(index >= NUMBER_OF_7_SEG_LED) return;

    HAL_GPIO_WritePin(SEG_A_GPIO_Port, SEG_A_Pin, !((buffer[index] >> 0) & 0x01));
    HAL_GPIO_WritePin(SEG_B_GPIO_Port, SEG_B_Pin, !((buffer[index] >> 1) & 0x01));
    HAL_GPIO_WritePin(SEG_C_GPIO_Port, SEG_C_Pin, !((buffer[index] >> 2) & 0x01));
    HAL_GPIO_WritePin(SEG_D_GPIO_Port, SEG_D_Pin, !((buffer[index] >> 3) & 0x01));
    HAL_GPIO_WritePin(SEG_E_GPIO_Port, SEG_E_Pin, !((buffer[index] >> 4) & 0x01));
    HAL_GPIO_WritePin(SEG_F_GPIO_Port, SEG_F_Pin, !((buffer[index] >> 5) & 0x01));
    HAL_GPIO_WritePin(SEG_G_GPIO_Port, SEG_G_Pin, !((buffer[index] >> 6) & 0x01));
}


void led7_refresh(void){
    static uint8_t currentIndex = 0;
    // update one digit per call
    update_LED7_driver(currentIndex);
    currentIndex++;
    if(currentIndex >= NUMBER_OF_7_SEG_LED) currentIndex = 0;
}

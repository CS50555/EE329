#ifndef KEYPAD_H_
#define KEYPAD_H_

//define rows and column numbers
#define ROWS 4
#define COLS 4
#define ROWPORTS GPIOC
#define COLPORTS GPIOB
#define ROW_PINS GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3
#define COL_PINS GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3
#define NUM_ROWS 4
#define NUM_COLS 4
#define SETTLE 100
#define FALSE 0
#define TRUE 1
#define BIT0 1
#define KEY_ZERO 0
#define CODE_ZERO 0
#define NO_KEYPRESS 0
#define PUPDRST 0
#define OUTPUT_PINS GPIO_PIN_4| GPIO_PIN_5| GPIO_PIN_6| GPIO_PIN_8



#include "stm32l4xx_hal.h"


//function declaration for key pressed (return 1 for true, 0 for false)
uint8_t Key_Pressed(void);
//function declaration for key pressed (return char,)
uint32_t Which_Key_Pressed(void);


#endif

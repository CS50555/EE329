/*******************************************************************************
* EE 329 A2 KEYPAD INTERFACE
*******************************************************************************
* @file           : KEYPAD.c
* @brief          : Runs the functions from Keypad.c and outputs onto LEDs
* project         : EE 329 S'25 Assignment 2
* authors         : Justin Rosu & Brayden Daly  (JRBD)
* version         : 0.1
* date            : 250412
* compiler        : STM32CubeIDE v.1.12.0 Build: 14980_20230301_1550 (UTC)
* target          : NUCLEO-L4A6ZG
* clocks          : 4 MHz MSI to AHB2
* @attention      : (c) 2023 STMicroelectronics.  All rights reserved.
*******************************************************************************
* KEYPAD PLAN :
* set columns as outputs, rows as inputs w pulldowns
* loop:
* drive all columns HI read all rows
* if any row N is HI
*    set all columns LO
*    drive each column M HI alone
*    read row N until HI 🡪 pressed key loc’n = N, M
* key value = 3N+M+1 for 1..9,special case for *,0,#
*******************************************************************************
* KEYPAD WIRING 4 ROWS 4 COLS (pinout NUCLEO-L4A6ZG = L496ZG)
*      peripheral – Nucleo I/O
* keypad 1  COL 4 - PB0
* keypad 2  COL 3 - PB1
* keypad 3  COL 2 - PB2
* keypad 4  COL 1 - PB3
* keypad 5  ROW 4 - PC3
* keypad 6  ROW 3 - PC2
* keypad 7  ROW 2 - PC1
* keypad 8  ROW 1 - PC0
* OUTPUT LED BIT0 - PB4
* OUTPUT LED BIT1 - PB5
* OUTPUT LED BIT2 - PB6
* OUTPUT LED BIT3 - PB8
*******************************************************************************
* REVISION HISTORY
* 0.1 230318 JRBD  created, wires in breadboard, no keypad
* 0.2 230410 JRBD  made code to make keypad operational
* 0.3 230413 JRBD  implemented modularity
*******************************************************************************
*******************************************************************************
* 45678-1-2345678-2-2345678-3-2345678-4-2345678-5-2345678-6-2345678-7-234567 */
/* USER CODE END Header */
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
#define NO_KEYPRESS 15
#define PUPDRST 0
#define OUTPUT_PINS GPIO_PIN_4| GPIO_PIN_5| GPIO_PIN_6| GPIO_PIN_8
#include "stm32l4xx_hal.h"
//function declaration for key pressed (return 1 for true, 0 for false)
uint8_t Key_Pressed(void);
//function declaration for key pressed (return char,)
uint32_t Which_Key_Pressed(void);
int Debounce(void);
#endif

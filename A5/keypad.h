/*
 *******************************************************************************
 * EE 329 A2 KEYPAD SUPPORT
 *******************************************************************************
 * @file           : keypad.h
 * @brief          : keypad support functions and defintions
 * project         : EE 329 S'25 - Assignment A2
 * authors         : Kelvin Shi, Brayden Daly
 * version         : 1.0
 * date            : 2025/04/14
 * compiler        : STM32CubeIDE v.1.12.0 Build: 14980_20230301_1550 (UTC)
 * target          : NUCLEO-L4A6ZG
 * clocks          : 4 MHz MSI to AHB2
 * @attention      : (c) 2023 STMicroelectronics.  All rights reserved.
 *******************************************************************************
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef INC_KEYPAD_H_
#define INC_KEYPAD_H_

#endif /* INC_KEYPAD_H_ */

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/* Defined Variable ----------------------------------------------------------*/
#define ROW0_PIN 0x0004 //PE2
#define ROW1_PIN 0x0008 //PE3
#define ROW2_PIN 0x0010 //PE4
#define ROW3_PIN 0x0020 //PE5

#define COL1_PIN 0x0040 //PE6
#define COL2_PIN 0x0080 //PE7
#define COL3_PIN 0x0100 //PE8
#define COL4_PIN 0x0200 //PE9

#define ALL_ROWS 0x03c
#define ALL_COLS 0x3c0

#define NUM_ROWS 4
#define NUM_COLS 4
#define NO_KEYPRESS -1

#define FALSE 0
#define TRUE 1

#define NUM_ROWS 4
#define NUM_COLS 4
#define TIME_SPACE 40

/* Exported functions prototypes ---------------------------------------------*/
void keypad_Config(void);
uint8_t keypad_IsAnyKeyPressed(void);
uint8_t keypad_WhichKeyIsPressed(void);
uint8_t keypad_Debounce(void);
uint8_t keypad_getInput(void);

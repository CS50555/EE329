/*******************************************************************************
 * EE 329 A2 KEYPAD FUNCTIONS
 *******************************************************************************
 * @file           : keypad.c
 * @brief          : keypad configuration, debouncer, and key identification
 * project         : EE 329 S'25 - Assignment A2
 * authors         : Kelvin Shi, Brayden Daly
 * version         : 1.0
 * date            : 2025/04/14
 * compiler        : STM32CubeIDE v.1.12.0 Build: 14980_20230301_1550 (UTC)
 * target          : NUCLEO-L4A6ZG
 * clocks          : 4 MHz MSI to AHB2
 * @attention      : (c) 2023 STMicroelectronics.  All rights reserved.
 *******************************************************************************
 * KEYPAD WIRING 4 ROWS 4 COLS (pinout NUCLEO-L4A6ZG = L496ZG)
 *      peripheral – Nucleo I/O
 *
 * GRID LAYOUT:
 *        COL1 COL2 COL3 COL4
 * Row 1: "1" , "2" , "3", "A"
 * Row 1: "4" , "5" , "6", "B"
 * Row 1: "7" , "8" , "9", "C"
 * Row 1: "*" , "0" , "#", "D"
 *
 * PINOUT:
 * COL1 - D59 - PE6 - OUTPUT
 * COL2 - D41 - PE7 - OUTPUT
 * COL3 - D42 - OE8 - OUTPUT
 * COL4 - D6  - PE9 - OUTPUT
 * ROW1 - D60 - PE3 - INPUT - PDR
 * ROW2 - D56 - PE2 - INPUT - PDR
 * ROW3 - D56 - PE4 - INPUT - PDR
 * ROW4 - D58 - PE5 - INPUT - PDR
 *
 * ADDITIONAL INFORMATION
 * "*" - 0xE
 * "#" - 0xF
 *******************************************************************************
 */

#include "keypad.h"

/* -----------------------------------------------------------------------------
 * function : void keypad_Config(void)
 * INs      : none
 * OUTs     : none
 * action   : Initialize the STM32 ports to interface with the keypad. enables
 * 			  GPIO E ports, set rows as the inputs, columns as the outputs,
 * 			  high speed, push-pull outputs, and pull down resistor for inputs
 * authors  : Kelvin Shi, Brayden Daly
 * version  : 1.0
 * date     : 2025/04/23
 * -------------------------------------------------------------------------- */
void keypad_Config(void) {
	RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOEEN);

	GPIOE->MODER &= ~(GPIO_MODER_MODE2 | GPIO_MODER_MODE3
			| GPIO_MODER_MODE4 | GPIO_MODER_MODE5);
	GPIOE->MODER &= ~(GPIO_MODER_MODE6 | GPIO_MODER_MODE7
			| GPIO_MODER_MODE8 | GPIO_MODER_MODE9);
	GPIOE->MODER |= (GPIO_MODER_MODE6_0 | GPIO_MODER_MODE7_0
			| GPIO_MODER_MODE8_0 | GPIO_MODER_MODE9_0);

	GPIOE->OTYPER &= ~(GPIO_OTYPER_OT6 | GPIO_OTYPER_OT7 | GPIO_OTYPER_OT8
			| GPIO_OTYPER_OT9);

	GPIOE->OSPEEDR |= ~((3 << GPIO_OSPEEDR_OSPEED6_Pos)
	| (3 << GPIO_OSPEEDR_OSPEED7_Pos) | (3 << GPIO_OSPEEDR_OSPEED8_Pos)
			| (3 << GPIO_OSPEEDR_OSPEED9_Pos));

	GPIOE->PUPDR &= ~(GPIO_PUPDR_PUPD6 | GPIO_PUPDR_PUPD7
			| GPIO_PUPDR_PUPD8 | GPIO_PUPDR_PUPD9);
	GPIOE->PUPDR |= ((2 << GPIO_PUPDR_PUPD2_Pos) | (2 << GPIO_PUPDR_PUPD3_Pos)
			| (2 << GPIO_PUPDR_PUPD4_Pos) | (2 << GPIO_PUPDR_PUPD5_Pos));
}

/* -----------------------------------------------------------------------------
 * function : uint8_t keypad_WhichKeyIsPressed(void)
 * INs      : none
 * OUTs     : int - the value associated with the keypad press
 * action   : verifies a keypad press is present and determines which key
 * 			  is pressed. Calculate the value pressed and return the value
 * authors  : Kelvin Shi, Brayden Daly
 * version  : 1.1
 * date     : 2025/04/23
 * REVISION HISTORY
 * 1.1 2025/04/23 Kelvin Shi  refactored key decode to use array
 * -------------------------------------------------------------------------- */
uint8_t keypad_WhichKeyIsPressed(void) {
	int8_t iRow = 0, iCol = 0; // keypad row/col index
	int8_t bGotKey = 0;                  // bool for keypress: 0 = no press

	for (iRow = 0; iRow < NUM_ROWS; iRow++) {   // check all ROWS
		if (GPIOE->IDR & (1 << (2 + iRow))) {   // keypress in iRow!
			GPIOE->BRR = (ALL_COLS);            // set all cols LOW
			for (iCol = 0; iCol < NUM_COLS; iCol++) {   // 1 col at a time
				GPIOE->BSRR = (1 << (6 + iCol));        // set this col HI
				if ( GPIOE->IDR & (1 << (2 + iRow))) {  // keypress in iCol!
					bGotKey = 1;
					break;                              // exit for iCol loop
				}
			}
			if (bGotKey)
				break;
		}
	}

	// create LUT and index based on row/col index
	if (bGotKey) {
		uint8_t lookup[16] = {1,2,3,10,4,5,6,11,7,8,9,12,14,0,15,13};
		return lookup[NUM_ROWS*iRow + iCol];
	}
	return (NO_KEYPRESS);                     // unable to verify keypress
}

/* -----------------------------------------------------------------------------
 * function : uint8_t keypad_IsAnyKeyPressed(void)
 * INs      : none
 * OUTs     : int - a status value, 1 corresponding to key press detected and
 * 			  0 corresponding to no key press detected
 * action   : check to see if there is a keypress detected. return the status
 * authors  : Kelvin Shi, Brayden Daly
 * version  : 1.0
 * date     : 2025/04/14
 * -------------------------------------------------------------------------- */
uint8_t keypad_IsAnyKeyPressed(void) {
	GPIOE->BSRR = ALL_COLS;         	        // set all columns HI
	if ((GPIOE->IDR & (ALL_ROWS)) != 0)         // got a keypress!
		return (TRUE);
	else
		return (FALSE);                         // nope keypress
}

/* -----------------------------------------------------------------------------
 * function : uint8_t keypad_Debounce(void)
 * INs      : none
 * OUTs     : int - a verified status value, 1 corresponding to key press detected
 * 			  and 0 corresponding to no key press detected
 * action   : verifies that keypress is actually received by performing 4 checks
 * 			  spaced 100 us apart. return true if verified keypress; else false
 * authors  : Kelvin Shi, Brayden Daly
 * version  : 1.0
 * date     : 2025/04/14
 * -------------------------------------------------------------------------- */
//will check on 4 instances spaced 100 us apart
uint8_t keypad_Debounce(void) {
	if (keypad_IsAnyKeyPressed()) {
		for (int i = 0; i < TIME_SPACE; i++)
			;
		if (keypad_IsAnyKeyPressed()) {
			for (int i = 0; i < TIME_SPACE; i++)
				;
			if (keypad_IsAnyKeyPressed()) {
				for (int i = 0; i < TIME_SPACE; i++)
					;
				if (keypad_IsAnyKeyPressed())
					return TRUE;
			}
		}
	}
	return FALSE;
}

/* -----------------------------------------------------------------------------
 * function : uint8_t keypad_getInput(void)
 * INs      : none
 * OUTs     : uint8_t - a confirmed value of the button pressed on the keypad
 * action   : block until user input from the keypad is received. return the
 * 			  value of the keypad pressed
 * authors  : Kelvin Shi, Brayden Daly
 * version  : 1.0
 * date     : 2025/04/22
 * -------------------------------------------------------------------------- */
uint8_t keypad_getInput(void) {
	while (1) {
		if (keypad_IsAnyKeyPressed()) {
			if (keypad_Debounce()) {
				uint8_t key = keypad_WhichKeyIsPressed();
				while (keypad_IsAnyKeyPressed());     //wait for press release
				return key;
			}
		}
	}
}

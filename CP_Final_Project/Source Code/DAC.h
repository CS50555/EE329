/*******************************************************************************
* EE 329 A5 DAC INTERFACE
*******************************************************************************
* @file           : DAC.h
* @brief          : Runs the functions from Keypad.c and outputs onto LEDs
* project         : EE 329 S'25 Assignment 2
* authors         : Justin Rosu & Stephanie Ly  (JRSL)
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
* CS - PA4
* CLCK - PB5
* SDI - PB7
*******************************************************************************
* REVISION HISTORY
* 0.1 230318 JRSL  created, wires in breadboard, no dac
* 0.2 230410 JRSL  made code to make dac operational
* 0.3 230413 JRSL  implemented modularity
*******************************************************************************
*******************************************************************************
* 45678-1-2345678-2-2345678-3-2345678-4-2345678-5-2345678-6-2345678-7-234567 */

#ifndef INC_DAC_H_
#define INC_DAC_H_

int DAC_volt_conv(float voltage );
void DAC_init(void);
void DAC_Write(uint16_t, uint8_t );
#endif /* INC_DAC_H_ */

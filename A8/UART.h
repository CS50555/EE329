/*
 * UART.h
 *
 *  Created on: May 7, 2025
 *      Author: bdaly, preston
 */

#ifndef UART_H_
#define UART_H_

#include "main.h"
#include "stm32l4xx.h"


#define UART_PORT GPIOG   	// Define the UART port as GPIOG
#define LEFT_BORDER 20		//Define Left border position
#define RIGHT_BORDER 140	//Define Right border position
#define UPPER_BORDER 35		//define upper border position
#define LOWER_BORDER 5		//define lower border position

// Function Prototypes
void LPUART_init(void);
void LPUART_Print(const char* message);
void LPUART1_IRQHandler(void);
void LPUART_ESC_Print(const char* esc_code, const char* message);
void Splash_Screen(void);
void Create_Border(void);
void print_current(uint16_t adc_val);
void print_voltage(uint16_t adc_val);
void print_uint4(uint16_t val);


#endif /* UART_H_ */

#ifndef INC_LCD_H_
#define INC_LCD_H_

#include "stm32l4xx_hal.h"
#include "delay.h"
#include "string.h"

#define LCD_LINE1	(0x80)
#define LCD_LINE2 (0xC0)
#define MOVE_CURSOR(row, col) (0x80 | ((row) == 0 ? (col) : (0x40 + (col))))
#define SHIFT_CURSOR_RIGHT (0x14)
#define LCD_DISPLAY_CTRL(D, C, B) (0x08 | ((D) << 2) | ((C) << 1) | (B))


#define LCD_PORT	(GPIOA)
#define LCD_RS  	(GPIO_PIN_0)
#define LCD_RW  	(GPIO_PIN_1)
#define LCD_EN  	(GPIO_PIN_2)

// D4–D7 connected to PA3–PA6
#define LCD_D4		(GPIO_PIN_3)
#define LCD_D5		(GPIO_PIN_4)
#define LCD_D6		(GPIO_PIN_5)
#define LCD_D7		(GPIO_PIN_6)

// Mask to clear/set LCD data bits quickly
#define LCD_DATA_BITS	(LCD_D4 | LCD_D5 | LCD_D6 | LCD_D7)

void LCD_Print_String( char string[] );
void LCD_Clear(void);
void LCD_Setup( void );
void LCD_Init( void );
void LCD_Pulse_ENA( void );
void LCD_4b_Command( uint8_t command );
void LCD_Command( uint8_t command );
void LCD_Write_Char( uint8_t letter );

#endif /* INC_LCD_H_ */

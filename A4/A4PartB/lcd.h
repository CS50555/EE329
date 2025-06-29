/*
 * lcd.h
 *
 *  Created on: Apr 22, 2025
 *      Author: wongb
 */
#include<stdint.h>
#ifndef INC_LCD_H_
#define INC_LCD_H_

//#define LCD_PORT (GPIOA)
//#define LCD_PORT_EN (RCC_AHB2ENR_GPIOAEN)
//// CONTROL BITS in HIGH NIBBLE
//#define LCD_RS (0x10)
//#define LCD_RW (0x20)
//#define LCD_EN (0x40)
//// DATA BITS in LOW NIBBLE
//#define LCD_DATA_BITS (0x0F)
//// FUNCTION PROTOTYPES
//void LCD_init( void );
//void LCD_pulse_ENA( void );
//void LCD_4b_command( uint8_t command );
//void LCD_command( uint8_t command );
//void LCD_write_char( uint8_t letter );
//uint8_t LCD_swapNibbles( uint8_t dataByte );
//
//// parameterized macro format
//#define NAME(param) (expression using param)
//#define SQUARE(x) ((x) * (x))
//// set Nth bit
//#define SET_BIT(x, pos) (x |= (1U << pos))
//// clear Nth bit
//#define CLR_BIT(x, pos) (x &= (~(1U<< pos)))
//// toggle Nth bit
//#define TOGGLE_BIT(x, pos) (x ^= (1U<< pos))
//
//#define CHK_BIT(x, pos) (x & (1UL << pos) )
//#define GET_BIT(x, pos) ((x & ( 1 << pos)) >> pos)
//// swap nibbles
//#define SWAP_4B(x) (((x & 0x0F)<<4) | ((x & 0xF0)>>4))




#define LCD_PORT GPIOF

#define LCD_B4	GPIO_PIN_0
#define LCD_B5 	GPIO_PIN_1
#define LCD_B6 GPIO_PIN_2
#define LCD_B7 GPIO_PIN_3

#define LCD_RS GPIO_PIN_7
#define LCD_RW GPIO_PIN_8
#define LCD_EN  GPIO_PIN_9


#define LCD_DATA_BITS (0x0F)


#define LCD_clear_display (0x01) // 0000x0001
//define LCD_setcursor (0x10)
#define LCD_return_home (0x02) // 0000x0010
#define LCD_entry_mode_set (0x06) // 0000x0111
#define LCD_display_on (0x0F) // 0000x1111
#define LCD_function_set (0x28) // 0010x1000
void LCD_config(void);









void LCD_4b_command(uint8_t command);
void LCD_Pulse_ENA(void);
void LCD_Init(void);
void LCD_command(uint8_t command);
void LCD_write_char(uint8_t letter);
void LCD_Set_Cursor(uint8_t row, uint8_t col);
void LCD_write_string(const char*str);




#endif /* INC_LCD_H_ */

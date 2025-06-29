
#include "LCD.h"
#include "main.h"
#include "Delay.h"
#include "string.h"
#include "stm32l4xx.h"     // ← Must be first for STM32 registers (RCC, GPIOx, etc.)
#include <stdint.h>        // ← Needed for uint32_t, int8_t, etc.

/* -----------------------------------------------------------------------------
* function : LCD-Clear(void);
* INs      : none
* OUTs     : none
* action   : Clears the LCD
* authors  : Brayden Daly, Tyler Wong
* version  : 0.5
* date     : 253004
* -------------------------------------------------------------------------- */
void LCD_Clear(void) {
    LCD_command(0x01);
    delay_us(2000); // clear takes longer than usual
}

/* -----------------------------------------------------------------------------
* function : LCD_Init(void);
* INs      : none
* OUTs     : none
* action   : Initializes LCD to operate in 4-bit mode using standard sequence.
*            Sends configuration commands and prepares display for use.
* authors  : Tyler Wong, Brayden Daly
* version  : 0.5
* date     : 253004
* -------------------------------------------------------------------------- */
void LCD_Init(void) {
   delay_us(40000);  // Wait 40ms after power-up
   // Send wake-up command 3 times in 8-bit mode
   for (int idx = 0; idx < 3; idx++) {
      LCD_4b_command(0x30);
      delay_us(200);
   }
   LCD_4b_command(0x20);  // Set to 4-bit mode
   delay_us(40);
   // Configure function, display, entry mode
   LCD_command(LCD_function_set);     // Function set (4-bit, 2-line, 5x8)
   delay_us(40);
   LCD_command(0x10);                 // Cursor move settings
   delay_us(40);
   LCD_command(LCD_display_on);      // Display ON, cursor ON, blink ON
   delay_us(40);
   LCD_command(LCD_entry_mode_set);  // Increment cursor, no display shift
   delay_us(40);
   LCD_command(LCD_clear_display);   // Clear display
   delay_us(2000);
   LCD_command(LCD_return_home);     // Return home
   delay_us(40);
   LCD_command(0x80);                // Set DDRAM address to line 1
   delay_us(40);
}
/* -----------------------------------------------------------------------------
* function : LCD_pulse_ENA(void);
* INs      : none
* OUTs     : none
* action   : Generates enable pulse to latch data into LCD on falling edge.
* authors  : Brayden Daly, Tyler Wong
* version  : 0.5
* date     : 253004
* -------------------------------------------------------------------------- */
void LCD_pulse_ENA(void) {
   LCD_PORT->ODR |= LCD_EN;   // Enable high
   delay_us(20);              // Wait for signal setup
   LCD_PORT->ODR &= ~LCD_EN;  // Enable low (falling edge)
   delay_us(20);              // Wait for signal hold
}
/* -----------------------------------------------------------------------------
* function : LCD_4b_command(uint8_t command);
* INs      : 8-bit command (only high nibble is used)
* OUTs     : none
* action   : Sends only the high nibble of a command (used during LCD wake-up).
* authors  : Brayden Daly, Tyler Wong
* version  : 0.5
* date     : 253004
* -------------------------------------------------------------------------- */
void LCD_4b_command(uint8_t command) {
   LCD_PORT->ODR &= ~LCD_DATA_BITS;       // Clear data pins
   LCD_PORT->ODR |= (command >> 4);       // Output high nibble
   delay_us(20);
   LCD_pulse_ENA();                       // Latch it
}
/* -----------------------------------------------------------------------------
* function : LCD_command(uint8_t command);
* INs      : 8-bit command
* OUTs     : none
* action   : Sends a full 8-bit command to the LCD in 4-bit mode.
* authors  : Brayden Daly, Tyler Wong
* version  : 0.5
* date     : 253004
* -------------------------------------------------------------------------- */
void LCD_command(uint8_t command) {
   LCD_PORT->ODR &= ~LCD_DATA_BITS;
   LCD_PORT->ODR |= (command >> 4) & 0x0F;  // Send high nibble
   delay_us(20);
   LCD_pulse_ENA();
   delay_us(400);
   LCD_PORT->ODR &= ~LCD_DATA_BITS;
   LCD_PORT->ODR |= command & 0x0F;        // Send low nibble
   delay_us(20);
   LCD_pulse_ENA();
   delay_us(400);
}
/* -----------------------------------------------------------------------------
* function : LCD_write_char(uint8_t letter);
* INs      : 8-bit character
* OUTs     : none
* action   : Sends a single character to the LCD to display at the current cursor position.
* authors  : Tyler Wong, Brayden Daly
* version  : 0.5
* date     : 253004
* -------------------------------------------------------------------------- */
void LCD_write_char(uint8_t letter) {
   LCD_PORT->ODR |= LCD_RS;        // RS = data mode
   LCD_PORT->ODR &= ~LCD_RW;       // RW = write mode
   delay_us(20);
   LCD_command(letter);            // Send character
   LCD_PORT->ODR &= ~LCD_RS;       // RS = command mode
}
/* -----------------------------------------------------------------------------
* function : LCD_write_string(const char* str);
* INs      : Pointer to null-terminated string
* OUTs     : none
* action   : Writes a full string to the LCD by sending characters one by one.
* authors  : Brayden Daly, Tyler Wong
* version  : 0.5
* date     : 253004
* -------------------------------------------------------------------------- */
void LCD_write_string(const char* str) {
   for (int i = 0; i < strlen(str); i++) {
      LCD_write_char(str[i]);
      delay_us(50);
   }
}
/* -----------------------------------------------------------------------------
* function : LCD_Set_Cursor(uint8_t row, uint8_t col);
* INs      : row (0 or 1), column (0-15)
* OUTs     : none
* action   : Sets LCD cursor to specified row and column.
* authors  : Brayden Daly
* version  : 0.5
* date     : 253004
* -------------------------------------------------------------------------- */
void LCD_Set_Cursor(uint8_t row, uint8_t col) {
   uint8_t address;
   switch (row) {
      case 0:
         address = 0x80 + col;  // Line 1 starts at 0x80
         break;
      case 1:
         address = 0xC0 + col;  // Line 2 starts at 0xC0
         break;
      default:
         return;  // Invalid row
   }
   LCD_command(address);
   delay_us(40);
}
/* -----------------------------------------------------------------------------
* function : LCD_config(void);
* INs      : none
* OUTs     : none
* action   : Configures GPIOF pins connected to LCD as output push-pull.
* authors  : Brayden Daly, Tyler Wong
* version  : 0.5
* date     : 253004
* -------------------------------------------------------------------------- */
void LCD_config(void) {
   // Enable GPIOF peripheral clock
   RCC->AHB2ENR |= RCC_AHB2ENR_GPIOFEN;
   // Set PF0-PF3 and PF7-PF9 as general purpose outputs (MODER = 01)
   LCD_PORT->MODER &= ~(GPIO_MODER_MODE0 |
                        GPIO_MODER_MODE1 |
                        GPIO_MODER_MODE2 |
                        GPIO_MODER_MODE3 |
                        GPIO_MODER_MODE7 |
                        GPIO_MODER_MODE8 |
                        GPIO_MODER_MODE9);

   LCD_PORT->MODER |=  (GPIO_MODER_MODE0_0 |
                        GPIO_MODER_MODE1_0 |
                        GPIO_MODER_MODE2_0 |
                        GPIO_MODER_MODE3_0 |
                        GPIO_MODER_MODE7_0 |
                        GPIO_MODER_MODE8_0 |
                        GPIO_MODER_MODE9_0);

   // Set output type to push-pull
   LCD_PORT->OTYPER &= ~(GPIO_OTYPER_OT0 |
                         GPIO_OTYPER_OT1 |
                         GPIO_OTYPER_OT2 |
                         GPIO_OTYPER_OT3 |
                         GPIO_OTYPER_OT7 |
                         GPIO_OTYPER_OT8 |
                         GPIO_OTYPER_OT9);

   // Disable pull-up/pull-down resistors
   LCD_PORT->PUPDR &= ~(GPIO_PUPDR_PUPD0 |
                        GPIO_PUPDR_PUPD1 |
                        GPIO_PUPDR_PUPD2 |
                        GPIO_PUPDR_PUPD3 |
                        GPIO_PUPDR_PUPD7 |
                        GPIO_PUPDR_PUPD8 |
                        GPIO_PUPDR_PUPD9);

   // Set GPIO speed to low for PF pins
   LCD_PORT->OSPEEDR &= ~(3 << GPIO_OSPEEDR_OSPEED0_Pos |
                          3 << GPIO_OSPEEDR_OSPEED1_Pos |
                          3 << GPIO_OSPEEDR_OSPEED2_Pos |
                          3 << GPIO_OSPEEDR_OSPEED3_Pos |
                          3 << GPIO_OSPEEDR_OSPEED7_Pos |
                          3 << GPIO_OSPEEDR_OSPEED8_Pos |
                          3 << GPIO_OSPEEDR_OSPEED9_Pos);
   // Reset all LCD control and data lines to LOW
   LCD_PORT->BRR = (GPIO_MODER_MODE0 |
                    GPIO_MODER_MODE1 |
                    GPIO_MODER_MODE2 |
                    GPIO_MODER_MODE3 |
                    GPIO_MODER_MODE7 |
                    GPIO_MODER_MODE8 |
                    GPIO_MODER_MODE9);
}


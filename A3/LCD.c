#include "lcd.h"

/* -----------------------------------------------------------------------------
 * function : LCD_print_string( )
 * INs      : none
 * OUTs     : none
 * action   : print the string (character by character) to lcd
 * authors  : Michael Brandon Lee , Brayden Daly
 * -------------------------------------------------------------------------- */
void LCD_print_string( char string[] ) {
	for ( uint8_t i = 0; i < strlen(string); i++ )
		LCD_write_char( string[i] );
}

/* -----------------------------------------------------------------------------
 * function : LCD_clear( )
 * INs      : none
 * OUTs     : none
 * action   : clear the LCD and delay after
 * authors  : Michael Brandon Lee , Brayden Daly
 * -------------------------------------------------------------------------- */
void LCD_clear(void) {
    LCD_command(0x01);
    delay_us(2000); // clear takes longer than usual
}

/* -----------------------------------------------------------------------------
 * function : LCD_home( )
 * INs      : none
 * OUTs     : none
 * action   : go home on LCD and delay after
 * authors  : Michael Brandon Lee , Brayden Daly
 * -------------------------------------------------------------------------- */
void LCD_home(void) {
    LCD_command(0x02);
    delay_us(2000); // clear takes longer than usual
}

/* -----------------------------------------------------------------------------
 * function : LCD_setup( )
 * INs      : none
 * OUTs     : none
 * action   : set GPIOA pins 0-7 to being outputs
 * authors  : Michael Brandon Lee , Brayden Daly
 * -------------------------------------------------------------------------- */
 void LCD_setup( void ) {
	// enable GPIOA for LCD
	RCC->AHB2ENR   |=  (RCC_AHB2ENR_GPIOAEN);
	// reset output mode
	LCD_PORT->MODER   &= ~(GPIO_MODER_MODE0 | GPIO_MODER_MODE1 | GPIO_MODER_MODE2 |
			GPIO_MODER_MODE3 | GPIO_MODER_MODE4 | GPIO_MODER_MODE5 |
			GPIO_MODER_MODE6);
	// set mode to output
	LCD_PORT->MODER   |=  (GPIO_MODER_MODE0_0 | GPIO_MODER_MODE1_0 |
			GPIO_MODER_MODE2_0 | GPIO_MODER_MODE3_0 | GPIO_MODER_MODE4_0 |
			GPIO_MODER_MODE5_0 | GPIO_MODER_MODE6_0);
	//
	LCD_PORT->OTYPER  &= ~(GPIO_OTYPER_OT0 | GPIO_OTYPER_OT1 | GPIO_OTYPER_OT2 |
			GPIO_OTYPER_OT3 | GPIO_OTYPER_OT4 | GPIO_OTYPER_OT5 | GPIO_OTYPER_OT6);
	// set all pins to no pull up or pull down
	LCD_PORT->PUPDR   &= ~(GPIO_PUPDR_PUPD0 | GPIO_PUPDR_PUPD1 | GPIO_PUPDR_PUPD2 |
			GPIO_PUPDR_PUPD3 | GPIO_PUPDR_PUPD4 | GPIO_PUPDR_PUPD5 |
			GPIO_PUPDR_PUPD6);
	// set all pin speeds to high
	LCD_PORT->OSPEEDR |=  ((3 << GPIO_OSPEEDR_OSPEED0_Pos) |
			(3 << GPIO_OSPEEDR_OSPEED1_Pos) |
			(3 << GPIO_OSPEEDR_OSPEED2_Pos) |
			(3 << GPIO_OSPEEDR_OSPEED3_Pos) |
			(3 << GPIO_OSPEEDR_OSPEED4_Pos) |
			(3 << GPIO_OSPEEDR_OSPEED5_Pos) |
			(3 << GPIO_OSPEEDR_OSPEED6_Pos));
	// reset all pins to 0
	LCD_PORT->BRR = (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |
			GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6);
 }

// ------------------------------------------------------ excerpt from lcd.c ---
 /* -----------------------------------------------------------------------------
  * function : LCD_Init( )
  * INs      : none
  * OUTs     : none
  * action   : initialize the LCD and set correct 4 bit mode
  * authors  : Michael Brandon Lee , Brayden Daly
  * -------------------------------------------------------------------------- */
void LCD_init( void )  {   // RCC & GPIO config removed - leverage A1, A2 code
   delay_us( 40000 );                     // power-up wait 40 ms
   for ( int idx = 0; idx < 3; idx++ ) {  // wake up 1,2,3: DATA = 0011 XXXX
      LCD_4b_command( 0x30 );             // HI 4b of 8b cmd, low nibble = X
      delay_us( 200 );
   }
   LCD_4b_command( 0x20 ); // fcn set #4: 4b cmd set 4b mode - next 0x28:2-line

   LCD_command(0x28); // Function Set: 4-bit, 2-line, 5x8 font

	LCD_command(0x08); // Display OFF

	LCD_clear();

	LCD_command(0x06); // Entry Mode Set: increment, no shift

	LCD_command(0x0C); // Display ON, cursor off, blink off
}

/* -----------------------------------------------------------------------------
 * function : LCD_pulse_ENA( )
 * INs      : none
 * OUTs     : none
 * action   : Enable the LCD
 * authors  : Michael Brandon Lee , Brayden Daly
 * -------------------------------------------------------------------------- */
void LCD_pulse_ENA( void )  {
	// ENAble line sends command on falling edge
	// set to restore default then clear to trigger
   LCD_PORT->ODR   |= ( LCD_EN );         // ENABLE = HI
   delay_us( 5 );                         // TDDR > 320 ns
   LCD_PORT->ODR   &= ~( LCD_EN );        // ENABLE = LOW
   delay_us( 5 );                         // low values flakey, see A3:p.1
}

/* -----------------------------------------------------------------------------
 * function : LCD_4b_command( )
 * INs      : uint8_t
 * OUTs     : none
 * action   : enable the LCD into 4 bit mode
 * authors  : Michael Brandon Lee , Brayden Daly
 * -------------------------------------------------------------------------- */
void LCD_4b_command( uint8_t command )  {
	// LCD command using high nibble only - used for 'wake-up' 0x30 commands
   LCD_PORT->ODR   &= ~( LCD_DATA_BITS ); 	// clear DATA bits
   LCD_PORT->ODR   |= ( (command >> 4) << 3 );   // DATA = command
   delay_us( 5 );
   LCD_pulse_ENA( );
}

/* -----------------------------------------------------------------------------
 * function : LCD_command( )
 * INs      : uint8_t
 * OUTs     : none
 * action   : write a comand to LCD in 4 bit mode
 * authors  : Michael Brandon Lee , Brayden Daly
 * -------------------------------------------------------------------------- */
void LCD_command( uint8_t command )  {
	// send command to LCD in 4-bit instruction mode
	// HIGH nibble then LOW nibble, timing sensitive
	LCD_PORT->ODR   &= ~( LCD_DATA_BITS );	// isolate cmd bits
   // HIGH shifted low
   LCD_PORT->ODR   |= ( ((command >> 4) << 3) & LCD_DATA_BITS );
   delay_us( 5 );
   // latch HIGH NIBBLE
   LCD_pulse_ENA( );
   // isolate cmd bits
   LCD_PORT->ODR   &= ~( LCD_DATA_BITS );
   // LOW nibble
   LCD_PORT->ODR   |= ( ((command & 0x0F) << 3) & LCD_DATA_BITS );
   delay_us( 5 );
   // latch LOW NIBBLE
   LCD_pulse_ENA( );
   delay_us( 40 );
}

/* -----------------------------------------------------------------------------
 * function : LCD_write_char( )
 * INs      : none
 * OUTs     : uint8_t
 * action   : Write a character to the LCD
 * authors  : Michael Brandon Lee , Brayden Daly
 * -------------------------------------------------------------------------- */
void LCD_write_char( uint8_t letter )  {
	// calls LCD_command() w/char data; assumes all ctrl bits set LO in LCD_init()
   LCD_PORT->ODR   |= ( LCD_RS );      // RS = HI for data to address
   delay_us( 5 );
   LCD_command( letter );             	// character to print
   LCD_PORT->ODR   &= ~( LCD_RS );     // RS = LO
   delay_us( 40 ); // ~40us required per datasheet
}

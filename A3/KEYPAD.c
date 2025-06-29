#include "keypad.h"

//table for mapping which key pressed
const char keypad_char_table[16] = {
    '0',  // 0x0
    '1',  // 0x1
    '2',  // 0x2
    '3',  // 0x3
    '4',  // 0x4
    '5',  // 0x5
    '6',  // 0x6
    '7',  // 0x7
    '8',  // 0x8
    '9',  // 0x9
    'A',  // 0xA
    'B',  // 0xB
    'C',  // 0xC
    'D',  // 0xD
    '*',  // 0xE
    '#'   // 0xF
};

//table for mapping which key pressed
const uint8_t keypad_decode_table[16] = {
    0x1,  // 0
    0x2,  // 1
    0x3,  // 2
    0xA,  // 3
    0x4,  // 4
    0x5,  // 5
    0x6,  // 6
    0xB,  // 7
    0x7,  // 8
    0x8,  // 9
    0x9,  // 10
    0xC,  // 11
    0xE,  // 12 (*)
    0x0,  // 13
    0xF,  // 14 (#)
    0xD   // 15
};


/* -----------------------------------------------------------------------------
 * function : Keypad_Config( )
 * INs      : none
 * OUTs     : none
 * action   : configure the clocks, GPIO I/O mode types, and PUPDR for keypad
 * authors  : Michael Brandon Lee, Brayden Daly
 * -------------------------------------------------------------------------- */
void Keypad_Config(void) {
	// config pins for keypad
	// using PD0-7 for pins 0-7 on keypad
	// PD0-3 are inputs, PD4-7 outputs
	// pull down resistors on only PD4-7
	RCC->AHB2ENR 	|= (RCC_AHB2ENR_GPIODEN);
	GPIOD->MODER 	&= ~(GPIO_MODER_MODE0 | GPIO_MODER_MODE1 | GPIO_MODER_MODE2 |
			GPIO_MODER_MODE3 | GPIO_MODER_MODE4 | GPIO_MODER_MODE5 |
			GPIO_MODER_MODE6 | GPIO_MODER_MODE7);
	GPIOD->MODER 	|= (GPIO_MODER_MODE0_0 | GPIO_MODER_MODE1_0 |
			GPIO_MODER_MODE2_0 | GPIO_MODER_MODE3_0);
	GPIOD->OTYPER 	&= ~(GPIO_OTYPER_OT0 | GPIO_OTYPER_OT1 | GPIO_OTYPER_OT2 |
			GPIO_OTYPER_OT3 | GPIO_OTYPER_OT4 | GPIO_OTYPER_OT5 |
			GPIO_OTYPER_OT6 | GPIO_OTYPER_OT7);
	GPIOD->PUPDR 	&= ~(GPIO_PUPDR_PUPD0 | GPIO_PUPDR_PUPD1 | GPIO_PUPDR_PUPD2 |
			GPIO_PUPDR_PUPD3 | GPIO_PUPDR_PUPD4 | GPIO_PUPDR_PUPD5 |
			GPIO_PUPDR_PUPD6 | GPIO_PUPDR_PUPD7);
	GPIOD->PUPDR 	|= (GPIO_PUPDR_PUPD4_1 | GPIO_PUPDR_PUPD5_1 |
			GPIO_PUPDR_PUPD6_1 | GPIO_PUPDR_PUPD7_1);
	GPIOD->OSPEEDR	|= ((3 << GPIO_OSPEEDR_OSPEED0_Pos) |
			(3 << GPIO_OSPEEDR_OSPEED1_Pos) | (3 << GPIO_OSPEEDR_OSPEED2_Pos) |
			(3 << GPIO_OSPEEDR_OSPEED3_Pos) | (3 << GPIO_OSPEEDR_OSPEED4_Pos) |
			(3 << GPIO_OSPEEDR_OSPEED5_Pos) | (3 << GPIO_OSPEEDR_OSPEED6_Pos) |
			(3 << GPIO_OSPEEDR_OSPEED7_Pos));

	// preset PD0-3 to 0
	GPIOD->BRR = (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
}


/* -----------------------------------------------------------------------------
 * function : keypad_WaitForPress( )
 * INs      : none
 * OUTs     : uint8_t
 * action   : Determine with debounce if key is actually pressed
 * authors  : Michael Brandon Lee, Brayden Daly
 * -------------------------------------------------------------------------- */
uint8_t keypad_WaitForPress(void) {
	uint8_t value = 0, last_value = 0xff;
	int32_t consec_reads = 0;
	// keep looping until keypress is detected
	while(1) {
		if(keypad_IsAnyKeyPressed()) {
			// update last value before overwriting value
			last_value = value;
			value = 1;
			// check if value is the same as the last value
			if (value == last_value) {
				if (consec_reads < 200)
					consec_reads++;
			}
			else
				consec_reads = 0;

			if (consec_reads == DEBOUNCE_LOOP) {
				//return
				return keypad_WhichKeyPressed();
			}
		}
		else {
			// update last value before overwriting value
			last_value = value;
			value = 0;
			if (value == last_value) {
				if (consec_reads > -200)
					consec_reads--;
			}
			else
				consec_reads = 0;
		}
	}
}


/* -----------------------------------------------------------------------------
 * function : keypad_WaitForRelease( )
 * INs      : none
 * OUTs     : none
 * action   : wait to see if key is released (debounce)
 * authors  : Michael Brandon Lee, Brayden Daly
 * -------------------------------------------------------------------------- */
void keypad_WaitForRelease(void) {
	uint8_t value = 0, last_value = 0xff;
	int32_t consec_reads = 0;
	// keep looping until keypress is detected
	while(1) {
		if(keypad_IsAnyKeyPressed()) {
			// update last value before overwriting value
			last_value = value;
			value = 1;
			// check if value is the same as the last value
			if (value == last_value) {
				if (consec_reads < 200)
					consec_reads++;
			}
			else
				consec_reads = 0;
		}
		else {
			// update last value before overwriting value
			last_value = value;
			value = 0;
			if (value == last_value) {
				if (consec_reads > -200)
					consec_reads--;
			}
			else
				consec_reads = 0;
			if (consec_reads == -DEBOUNCE_LOOP) {
				// end loop
				break;
			}
		}
	}
}

/* -----------------------------------------------------------------------------
 * function : keypad_IsAnyKeyPressed( )
 * INs      : none
 * OUTs     : uint8_t
 * action   : Check if any key is pressed and return 1 if it is and 0 if not
 * authors  : Michael Brandon Lee, Brayden Daly
 * -------------------------------------------------------------------------- */
uint8_t keypad_IsAnyKeyPressed(void) {
	// drive all COLUMNS HI; see if any ROWS are HI
	// return true if a key is pressed, false if not
	GPIOD->BSRR = (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
	// give time for voltage to settle
	for (uint16_t time = 0; time < 1000; time++)
		;
	// check if any port is 0
	if ((GPIOD->IDR & (GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7)) != 0)
		return (1);
	else
		return (0);
}

/* -----------------------------------------------------------------------------
 * function : keypad_WhichKeyPressed( )
 * INs      : none
 * OUTs     : uint8_t
 * action   : check which key on keypad is pressed and return that value
 * authors  : Michael Brandon Lee, Brayden Daly
 * -------------------------------------------------------------------------- */
uint8_t keypad_WhichKeyPressed(void) {
	// keep track if key is found
	uint8_t gotKey = 0;
	uint8_t row = 0, column = 0;
	// reset column pins to 1
	GPIOD->BSRR = (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
	// find which row is pressed
	for (row = 0; row < 4; row++) {
		if (GPIOD->IDR & (GPIO_PIN_4 << row)) {
			// turn on each column 1 at a time
			for (column = 0; column < 4; column++) {
				// reset all columns and then turn on column
				GPIOD->BRR = (GPIO_PIN_0 | GPIO_PIN_1 |
				GPIO_PIN_2 | GPIO_PIN_3);
				GPIOD->BSRR = (GPIO_PIN_0 << column);
				// check if key is pressed
				if (GPIOD->IDR & (GPIO_PIN_4 << row)) {
					// if key pressed break out
					//of column loop
					gotKey = 1;
					break;
				}
			}
			if (gotKey)
				break;
		}
	}
	// decode row and column idx to overall idx
	uint8_t keypad_idx_pos = (row * 4) + column;
	//return keypad_idx_pos;
	// convert overall idx to values on keypad
	return keypad_decode_table[keypad_idx_pos];
}

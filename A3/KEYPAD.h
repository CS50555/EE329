#ifndef INC_KEYPAD_H_
#define INC_KEYPAD_H_

#include "stm32l4xx_hal.h"

#define DEBOUNCE_LOOP 20
#define NO_KEYPRESS (0xff)

extern const char keypad_char_table[16];

void Keypad_Config(void);
uint8_t keypad_WaitForPress(void);
void keypad_WaitForRelease(void);
uint8_t keypad_IsAnyKeyPressed(void);
uint8_t keypad_WhichKeyPressed(void);

#endif /* INC_KEYPAD_H_ */

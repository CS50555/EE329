#ifndef BUTTONS_H
#define BUTTONS_H

#include <stdint.h>

extern volatile uint8_t button_flags;

void Button_Init(void);
uint8_t Button_WasPressed(uint8_t pin,uint8_t audio_queue);


#endif


///*
// * BUTTONS.h
// *
// *  Created on: May 29, 2025
// *      Author: jrosu
// */
//
//#ifndef INC_BUTTONS_H_
//#define INC_BUTTONS_H_
//
//#define INPUT_PORT GPIOC
//#define DEBOUNCE_DELAY_MS 20
//uint8_t detect_Button_Press(uint8_t);
//void SysTick_Init(void);
//void Button_Configuration(void);
//
//#endif /* INC_BUTTONS_H_ */

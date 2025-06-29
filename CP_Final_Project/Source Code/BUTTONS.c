
/*
 * BUTTONS.c
 *
 *  Created on: May 29, 2025
 *      Author: jrosu
 */

#include <KEYPAD.h>
#include "BUTTONS.h"
#include "main.h"
int last_button_state[] = {0,0,0,0};

/* ---------------------------- (Button_Configuration ) ------------------------------
* Initializes pind D0-6 for input mode
* Inputs: None
* Outputs: None
* Local vars: None
* -------------------------------------------------------------------------- */
void Button_Configuration(void)
{
	  //Initialize clock and inputs and outputs and stuff
	  //Enable GPIOD clock for peripheral
	  RCC->AHB2ENR |= RCC_AHB2ENR_GPIODEN;
	  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	  //set GPIO to input mode (Reset bits 6 and 7 for input mode) (00)
	  GPIOD->MODER = 0;
	  //INITIALIZES ALL INPUTS
	  GPIOB->MODER &= ~(GPIO_MODER_MODE0 | GPIO_MODER_MODE1 | GPIO_MODER_MODE2 | GPIO_MODER_MODE3 | GPIO_MODER_MODE4|GPIO_MODER_MODE5|GPIO_MODER_MODE6 |GPIO_MODER_MODE8);
	  GPIOB->MODER |=  (GPIO_MODER_MODE0_0 | GPIO_MODER_MODE1_0 | GPIO_MODER_MODE2_0 | GPIO_MODER_MODE3_0 |GPIO_MODER_MODE4_0| GPIO_MODER_MODE5_0|GPIO_MODER_MODE6_0| GPIO_MODER_MODE8_0);
	  //Initializes PUPDR
	  GPIOD->PUPDR &= PUPDRST;
	  GPIOD->PUPDR |= (GPIO_PUPDR_PUPD0_1 | GPIO_PUPDR_PUPD1_1 | GPIO_PUPDR_PUPD2_1 | GPIO_PUPDR_PUPD3_1 |GPIO_PUPDR_PUPD4_1 |GPIO_PUPDR_PUPD5_1 |GPIO_PUPDR_PUPD6_1);
	  GPIOB->PUPDR |= (GPIO_PUPDR_PUPD0_0 | GPIO_PUPDR_PUPD1_0 | GPIO_PUPDR_PUPD2_0 | GPIO_PUPDR_PUPD3_0 | GPIO_PUPDR_PUPD3_0);
	  GPIOD->OTYPER &= (GPIO_OTYPER_OT0 | GPIO_OTYPER_OT1 |GPIO_OTYPER_OT2 |GPIO_OTYPER_OT3| GPIO_OTYPER_OT4 |GPIO_OTYPER_OT5 | GPIO_OTYPER_OT6);
}

/* ---------------------------- (detect_Button_Press ) ------------------------
* Checks if button is pressed and if sound is not already playing
* Inputs: pin, audio play
* Outputs: 1 or 0
* Local vars: None
* -------------------------------------------------------------------------- */
uint8_t detect_Button_Press(uint8_t pin, uint8_t audio_queue) {
    if (pin > 3) return 0;  // invalid

    if ((GPIOD->IDR & (1U << pin)) && (!audio_queue)){

   		 return 1;

    }
    return 0;

}









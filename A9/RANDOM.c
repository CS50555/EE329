/*
 * RANDOM.c
 *
 *  Created on: May 22, 2025
 *      Author: bdaly, alexn
 */

#include "main.h"
#include "delay.h"

void RNG_Init(void)
{
	//init RNG reg
	RCC->CRRCR |= RCC_CRRCR_HSI48ON;
	while ((RCC->CRRCR & RCC_CRRCR_HSI48RDY) == 0); // wait until it's ready
	RCC->AHB2ENR |= RCC_AHB2ENR_RNGEN;
	RNG->CR |= RNG_CR_RNGEN;
}
/* -----------------------------------------------------------------------------
* function : Get_Ran_Num()
* INs      : none
* OUTs     : random 16 bit number
* action   : random value generator
* authors  : Brayden Daly
* version  : 0.1
* date     : 250428
* -------------------------------------------------------------------------- */
uint16_t Get_Ran_Num(void) {
    while ((RNG->SR & RNG_SR_DRDY) == 0); // Wait until a number is ready
    uint16_t rand_num = RNG->DR;
    return rand_num;
}

/* -----------------------------------------------------------------------------
* function : On_Board_LED()
* INs      : none
* OUTs     : none
* action   : initilizes on board LED 1
* authors  : Brayden Daly, Alex N
* version  : 0.1
* date     : 250428
* -------------------------------------------------------------------------- */
void On_Board_LED(void) {
    // Enable clock for GPIOC (for PC7)
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
    // Set PC7 to output mode
    GPIOC->MODER &= ~(3U << (7 * 2));   // Clear bits
    GPIOC->MODER |=  (1U << (7 * 2));   // Set to output

    GPIOC->OTYPER  &= ~(1U << 7);       // Push-pull
    GPIOC->OSPEEDR |=  (3U << (7 * 2)); // High speed
    GPIOC->PUPDR   &= ~(3U << (7 * 2)); // No pull
}

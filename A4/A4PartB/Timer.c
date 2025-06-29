/* USER CODE BEGIN Header */
/*******************************************************************************
 * EE 329 A3 KEYPAD INTERFACE
 *******************************************************************************
 * @file       	: Timer.c
 * @brief      	: Timer and clock configuration
 * project     	: EE 329 S'23 Assignment 4
 * authors     	: Tyler Wong
 *              : Brayden Daly
 * version     	: 0.1
 * date        	: 250423
 * compiler    	: STM32CubeIDE v.1.12.0 Build: 14980_20230301_1550 (UTC)
 * target      	: NUCLEO-L4A6ZG
 * clocks      	: 4 MHz MSI to AHB2
 * @attention  	: (c) 2023 STMicroelectronics.  All rights reserved.
 *******************************************************************************
/* USER CODE END Header */

#include "main.h"
#include "Timer.h"

/* -----------------------------------------------------------------------------
* function : setup_TIM2( int iDutyCycle )
* INs      : CCR1 interrupt count
* OUTs     : none
* action   : Initializes TIM2 with infinite ARR and an input CCR1 count
* authors  : Tyler Wong (TW) - twong103@calpoly.edu
* version  : 0.1
* date     : 250430
* -------------------------------------------------------------------------- */
void setup_TIM2( int iDutyCycle ) {
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;           // enable clock for TIM2
    TIM2->DIER |= (TIM_DIER_CC1IE | TIM_DIER_UIE);  // enable event gen, rcv CCR1
    TIM2->ARR = 0xFFFFFFFF;                         // ARR = T = counts @4MHz
    TIM2->CCR1 = iDutyCycle - 1;                    // ticks for duty cycle
    TIM2->SR &= ~(TIM_SR_CC1IF | TIM_SR_UIF);       // clr IRQ flag in status reg
    NVIC->ISER[0] |= (1 << (TIM2_IRQn & 0x1F));     // set NVIC interrupt: 0x1F
    __enable_irq();                                 // global IRQ enable
    TIM2->CR1 |= TIM_CR1_CEN;                       // start TIM2 CR1
 }

/* -----------------------------------------------------------------------------
* function : setup_MCO_CLK(void)
* INs      : none
* OUTs     : none
* action   : Assigns the MCO clock to PA8 output
* authors  : Tyler Wong (TW) - twong103@calpoly.edu
* version  : 0.1
* date     : 250430
* -------------------------------------------------------------------------- */
void setup_MCO_CLK(void) {
   // enable MCO, MCOSEL = 0b0001 to select SYSCLK = MSI (4 MHz source)
   RCC->CFGR = ((RCC->CFGR & ~(RCC_CFGR_MCOSEL)) | (RCC_CFGR_MCOSEL_0));
   // configure MCO output on PA8
   RCC->AHB2ENR   |=  (RCC_AHB2ENR_GPIOAEN);
   GPIOA->MODER   &= ~(GPIO_MODER_MODE8);    	// clear MODER bits
   GPIOA->MODER   |=  (GPIO_MODER_MODE8_1);	    // MODE = alternate function
   GPIOA->OTYPER  &= ~(GPIO_OTYPER_OT8);     	// push-pull output
   GPIOA->PUPDR   &= ~(GPIO_PUPDR_PUPD8);    	// pullup & pulldown OFF
   GPIOA->OSPEEDR |=  (GPIO_OSPEEDR_OSPEED8);   // high speed
   GPIOA->AFR[1]  &= ~(GPIO_AFRH_AFSEL8);    	// select MCO (AF0) on PA8 (AFRH)
}

/* -----------------------------------------------------------------------------
* function : TIM2_IRQHandler(void)
* INs      : none
* OUTs     : PC0 for 5 kHz, 50% duty cycle square wave & PC1 for IQR execution
* 		   : time
* action   : Where the interrupts and executes GPIO outputs
* authors  : Tyler Wong (TW) - twong103@calpoly.edu
* version  : 0.1
* date     : 250430
* -------------------------------------------------------------------------- */
void TIM2_IRQHandler(void) {
	GPIOC->BSRR = (GPIO_PIN_1);		   // IQR execution time start
   if (TIM2->SR & TIM_SR_CC1IF) {      // triggered by CCR1 event
      TIM2->SR &= ~(TIM_SR_CC1IF);     // manage the flag
      GPIOC->ODR ^= (GPIO_PIN_0);      // Toggle PC0;
      TIM2->CCR1 += 400;			   // assign next CCR1 interrupt
     if(TIM2->CCR1 <= TIM2->CNT)	   // check if CCR1 is in the past
      TIM2->CCR1 = TIM2->CNT + 400;	   // reschedule the future
   }

   if (TIM2->SR & TIM_SR_UIF) {        // triggered by ARR event
      TIM2->SR &= ~(TIM_SR_UIF);       // manage the flag
      ;    							   // does nothing
   }
   GPIOC->BRR = (GPIO_PIN_1);		   // IQR execution time stop
}

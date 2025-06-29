#include "TIMER.h"
#include "main.h"


void setup_TIM2( int iDutyCycle ) {
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;           // enable clock for TIM2
    TIM2->DIER |= (TIM_DIER_CC1IE | TIM_DIER_UIE);  // enable event gen, rcv CCR1
    TIM2->ARR = PERIOD;                         // ARR = T = counts @4MHz
    TIM2->CCR1 = iDutyCycle - 1;                        // ticks for duty cycle
    TIM2->SR &= ~(TIM_SR_CC1IF | TIM_SR_UIF);       // clr IRQ flag in status reg
    NVIC->ISER[0] |= (1 << (TIM2_IRQn & 0x1F));     // set NVIC interrupt: 0x1F
    __enable_irq();                                 // global IRQ enable
    TIM2->CR1 |= TIM_CR1_CEN;                       // start TIM2 CR1
 }

void setup_MCO_CLK(void) {
   // enable MCO, MCOSEL = 0b0001 to select SYSCLK = MSI (4 MHz source)
   RCC->CFGR = ((RCC->CFGR & ~(RCC_CFGR_MCOSEL)) | (RCC_CFGR_MCOSEL_0));
   // configure MCO output on PA8
   RCC->AHB2ENR   |=  (RCC_AHB2ENR_GPIOAEN);
   GPIOA->MODER   &= ~(GPIO_MODER_MODE8);    	// clear MODER bits to set ...
   GPIOA->MODER   |=  (GPIO_MODER_MODE8_1);	    // MODE = alternate function
   GPIOA->OTYPER  &= ~(GPIO_OTYPER_OT8);     	// push-pull output
   GPIOA->PUPDR   &= ~(GPIO_PUPDR_PUPD8);    	// pullup & pulldown OFF
   GPIOA->OSPEEDR |=  (GPIO_OSPEEDR_OSPEED8);   // high speed
   GPIOA->AFR[1]  &= ~(GPIO_AFRH_AFSEL8);    	// select MCO (AF0) on PA8 (AFRH)
}


void TIM2_IRQHandler(void) {
	GPIOC->BSRR = (GPIO_PIN_1);
   if (TIM2->SR & TIM_SR_CC1IF) {      // triggered by CCR1 event ...
      TIM2->SR &= ~(TIM_SR_CC1IF);     // manage the flag
      GPIOC->ODR ^= (GPIO_PIN_0);      // Toggle PC0;
      TIM2->CCR1 += 400;
     if(TIM2->CCR1 <= TIM2->CNT)
      TIM2->CCR1 = TIM2->CNT + 400;
   }

   if (TIM2->SR & TIM_SR_UIF) {        // triggered by ARR event ...
      TIM2->SR &= ~(TIM_SR_UIF);       // manage the flag
      time_elapse ++;    							  // <-- manage GPIO pin here
   }
   GPIOC->BRR = (GPIO_PIN_1);
}
//void TIM2_IRQHandler(void) {
//
//   if (TIM2->SR & TIM_SR_CC1IF) {      // triggered by CCR1 event ...
//      TIM2->SR &= ~(TIM_SR_CC1IF);     // manage the flag
//      GPIOC->BRR = (GPIO_PIN_0);      // Toggle PC0;
//   }
//   if (TIM2->SR & TIM_SR_UIF) {        // triggered by ARR event ...
//      TIM2->SR &= ~(TIM_SR_UIF);       // manage the flag
//      GPIOC->BSRR = (GPIO_PIN_0);   // <-- manage GPIO pin here
//   }
//}

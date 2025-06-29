#include "TIMER.h"
#include "main.h"
#include "DAC.h"

uint16_t out1;
uint16_t out2;
uint16_t out3;
uint16_t out4;

/* ---------------------------- (Timer_setup_TIM2) ------------------------------
* Initializes TIM2 for periodic interrupts at a specified duty cycle.
* Enables TIM2 clock, sets ARR and CCR1 values, and configures NVIC interrupt.
* Inputs: int iDutyCycle - the duty cycle count to load into CCR1
* Outputs: None
* Local vars: None
* -------------------------------------------------------------------------- */
void Timer_setup_TIM2(int iDutyCycle) {
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;           // enable clock for TIM2
    TIM2->DIER |= (TIM_DIER_CC1IE | TIM_DIER_UIE);  // enable interrupt on CCR1 and overflow
    TIM2->ARR = 360;                                // auto-reload value for timing
    TIM2->CCR1 = iDutyCycle;                        // capture/compare register
    TIM2->SR &= ~(TIM_SR_CC1IF | TIM_SR_UIF);       // clear interrupt flags
    NVIC->ISER[0] |= (1 << (TIM2_IRQn & 0x1F));     // enable TIM2 IRQ in NVIC
    __enable_irq();                                 // enable global interrupts
    TIM2->CR1 |= TIM_CR1_CEN;                       // start TIM2
}

/* ---------------------------- (TIM2_IRQHandler) ------------------------------
* Interrupt Service Routine for TIM2.
* Handles both CCR1 (compare match) and ARR (update) events.
* On update, mixes and outputs audio samples to the DAC and increments counters.
* Inputs: None
* Outputs: None
* Local vars: out1, out2, out3, out4
* -------------------------------------------------------------------------- */
void TIM2_IRQHandler(void) {
    if (TIM2->SR & TIM_SR_CC1IF) {          // CCR1 interrupt
        TIM2->SR &= ~(TIM_SR_CC1IF);        // clear CCR1 flag
    }

    if (TIM2->SR & TIM_SR_UIF) {            // update interrupt (ARR reached)
        TIM2->SR &= ~(TIM_SR_UIF);          // clear update flag

        // Mix audio channels and output to DAC
        out1 = (audio_sample / 4) * audio_playing;
        out2 = (audio_sample2 / 4) * audio_playing2;
        out3 = (audio_sample3 / 4) * audio_playing3;
        out4 = (audio_sample4 / 4) * audio_playing4;

        // Sum channels, apply DC correction and write to DAC
        DAC_Write(out1 + out2 + out3 + out4 + ((-512) * (audio_playing + audio_playing2 + audio_playing3 + audio_playing4)) + 2048, 1);

        // Increment playback counters
        audio_counter++;
        audio_counter2++;
        audio_counter3++;
        audio_counter4++;
    }
}

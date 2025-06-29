#include "main.h"
#include "Delay.h"
#include "stm32l4xx.h"
#include <stdint.h>

/* -----------------------------------------------------------------------------
 * function : SysTick_Init(void);
 * INs      : none
 * OUTs     : none
 * action   : Configures the ARM Cortex-M SysTick timer for microsecond delays.
 *            Disables interrupts and sets it to use the processor clock.
 * authors  : Rodrigo Menchaca (rmr)
 * version  : 0.3
 * date     : 250421
 * -------------------------------------------------------------------------- */
void SysTick_Init(void) {
   SysTick->CTRL |= (SysTick_CTRL_ENABLE_Msk |       // Enable SysTick
                     SysTick_CTRL_CLKSOURCE_Msk);    // Use processor clock
   SysTick->CTRL &= ~(SysTick_CTRL_TICKINT_Msk);     // Disable SysTick interrupt
}

/* -----------------------------------------------------------------------------
 * function : delay_us(uint32_t time_us);
 * INs      : time_us - number of microseconds to delay
 * OUTs     : none (blocking delay)
 * action   : Uses SysTick countdown to delay for specified number of microseconds.
 *            Note: small values may result in longer-than-expected delay.
 * authors  : Rodrigo Menchaca (rmr)
 * version  : 0.3
 * date     : 250421
 * -------------------------------------------------------------------------- */
void delay_us(const uint32_t time_us) {
   // Calculate number of clock cycles for the desired delay
   SysTick->LOAD = (uint32_t)((time_us * (SystemCoreClock / 1000000)) - 1);
   SysTick->VAL = 0;                                     // Reset SysTick counter
   SysTick->CTRL &= ~(SysTick_CTRL_COUNTFLAG_Msk);       // Clear count flag
   while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)); // Wait for countdown
}

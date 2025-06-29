/*
 * SPI2.c
 *
 *  Created on: Jun 5, 2025
 *      Author: jrosu
 */


#include "main.h"
#include "stm32l4xx_hal.h"
/* ---------------------------- (SPI_Config) ------------------------------
* Initializes all things needed for the SPI to work
* Inputs: None
* Outputs: None
* Local vars: None
* -------------------------------------------------------------------------- */


void SPI2_Config(void){
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
    RCC->APB1ENR1 |= RCC_APB1ENR1_SPI2EN;

    // MODER to Alternate Function mode
    GPIOB->MODER &= ~((3U << (12 * 2)) | (3U << (13 * 2)) | (3U << (15 * 2)));
    GPIOB->MODER |=  ((2U << (12 * 2)) | (2U << (13 * 2)) | (2U << (15 * 2)));

    // Push-pull
    GPIOB->OTYPER &= ~((1U << 12) | (1U << 13) | (1U << 15));

    // Very high speed
    GPIOB->OSPEEDR |= (3U << (12 * 2)) | (3U << (13 * 2)) | (3U << (15 * 2));

    // No pull-up/pull-down
    GPIOB->PUPDR &= ~((3U << (12 * 2)) | (3U << (13 * 2)) | (3U << (15 * 2)));

    // AFR[1] = AF5 (SPI2)
    GPIOB->AFR[1] &= ~((0xF << ((12 - 8) * 4)) |
                       (0xF << ((13 - 8) * 4)) |
                       (0xF << ((15 - 8) * 4)));

    GPIOB->AFR[1] |=  ((5U << ((12 - 8) * 4)) |
                       (5U << ((13 - 8) * 4)) |
                       (5U << ((15 - 8) * 4)));
}


/* ---------------------------- (SPI_init) ------------------------------
* Initializes all things needed for the SPI to work
* Inputs: None
* Outputs: None
* Local vars: None
* -------------------------------------------------------------------------- */

void SPI2_init( void ) {
   // SPI config as specified @ STM32L4 RM0351 rev.9 p.1459
   // called by or with DAC_init()
   // build control registers CR1 & CR2 for SPI control of peripheral DAC
   // assumes no active SPI xmits & no recv data in process (BSY=0)
   // CR1 (reset value = 0x0000)
   SPI2->CR1 &= ~( SPI_CR1_SPE );             	// disable SPI for config
   SPI2->CR1 &= ~( SPI_CR1_RXONLY );          	// recv-only OFF
   SPI2->CR1 &= ~( SPI_CR1_LSBFIRST );        	// data bit order MSb:LSb
   SPI2->CR1 &= ~( SPI_CR1_CPOL | SPI_CR1_CPHA ); // SCLK polarity:phase = 0:0
   SPI2->CR1 |=	 SPI_CR1_MSTR;              	// MCU is SPI controller
   // CR2 (reset value = 0x0700 : 8b data)
   SPI2->CR2 &= ~( SPI_CR2_TXEIE | SPI_CR2_RXNEIE ); // disable FIFO intrpts
   SPI2->CR2 &= ~( SPI_CR2_FRF);              	// Moto frame format
   SPI2->CR2 |=	 SPI_CR2_NSSP;              	// auto-generate NSS pulse
   SPI2->CR2 |=	 SPI_CR2_DS;                	// 16-bit data
   SPI2->CR2 |=	 SPI_CR2_SSOE;              	// enable SS output// CR1
   SPI2->CR1 |=	 SPI_CR1_SPE;               	// re-enable SPI for ops
}

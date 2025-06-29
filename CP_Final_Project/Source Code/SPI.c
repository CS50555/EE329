#include "main.h"
#include "stm32l4xx_hal.h"
/* ---------------------------- (SPI_Config) ------------------------------
* Initializes all things needed for the SPI to work
* Inputs: None
* Outputs: None
* Local vars: None
* -------------------------------------------------------------------------- */


void SPI_Config(void){

	RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOAEN);                // GPIOA: DAC NSS/SCK/SDO
	RCC->APB2ENR |= (RCC_APB2ENR_SPI1EN);                 // SPI1 port
	/* USER ADD GPIO configuration of MODER/PUPDR/OTYPER/OSPEEDR registers HERE */
	// configure AFR for SPI1 function (1 of 3 SPI bits shown here)
	// set MODER: AF mode for PA5, PA7; Output mode for PA4
	GPIOA->MODER &= ~(GPIO_MODER_MODE4 |GPIO_MODER_MODE5| GPIO_MODER_MODE7 );
	GPIOA->MODER |=  ((2 << 8) | (2 << 10) | (2 << 14));
	// set OTYPER: push-pull for all SPI pins
	GPIOA->OTYPER &= ~(GPIO_OTYPER_OT4 | GPIO_OTYPER_OT5 | GPIO_OTYPER_OT7);

	// set OSPEEDR: high speed for all SPI pins''
	GPIOA->OSPEEDR &=  ~((GPIO_OSPEEDR_OSPEED4_Pos) | (GPIO_OSPEEDR_OSPEED5_Pos) | (GPIO_OSPEEDR_OSPEED7_Pos));
	GPIOA->OSPEEDR |= ((3 << GPIO_OSPEEDR_OSPEED4_Pos) |( 3<< GPIO_OSPEEDR_OSPEED5_Pos) | (3 << (GPIO_OSPEEDR_OSPEED7_Pos)));

	// set PUPDR: no pull-up/pull-down
   //GPIOA->PUPDR &= ~((3U << (4 * 2)) | (3U << (5 * 2)) | (3U << (7 * 2)));

	GPIOA->AFR[0] &= ~((0x000F << GPIO_AFRL_AFSEL7_Pos)); // clear nibble for bit 7 AF
	GPIOA->AFR[0] |=  ((0x0005 << GPIO_AFRL_AFSEL7_Pos)); // set b7 AF to SPI1 (fcn 5)
	GPIOA->AFR[0] &= ~((0x000F << GPIO_AFRL_AFSEL5_Pos)); // clear nibble for bit 5 AF
	GPIOA->AFR[0] |=  ((0x0005 << GPIO_AFRL_AFSEL5_Pos)); // set b5 AF to SPI1 (fcn 5)
	GPIOA->AFR[0] &= ~((0x000F << GPIO_AFRL_AFSEL4_Pos)); // clear nibble for bit 4 AF
	GPIOA->AFR[0] |=  ((0x0005 << GPIO_AFRL_AFSEL4_Pos)); // set b4 AF to SPI1 (fcn 5)
}

/* ---------------------------- (SPI_init) ------------------------------
* Initializes all things needed for the SPI to work
* Inputs: None
* Outputs: None
* Local vars: None
* -------------------------------------------------------------------------- */

void SPI_init( void ) {
   // SPI config as specified @ STM32L4 RM0351 rev.9 p.1459
   // called by or with DAC_init()
   // build control registers CR1 & CR2 for SPI control of peripheral DAC
   // assumes no active SPI xmits & no recv data in process (BSY=0)
   // CR1 (reset value = 0x0000)
   SPI1->CR1 &= ~( SPI_CR1_SPE );             	// disable SPI for config
   SPI1->CR1 &= ~( SPI_CR1_RXONLY );          	// recv-only OFF
   SPI1->CR1 &= ~( SPI_CR1_LSBFIRST );        	// data bit order MSb:LSb
   SPI1->CR1 &= ~( SPI_CR1_CPOL | SPI_CR1_CPHA ); // SCLK polarity:phase = 0:0
   SPI1->CR1 |=	 SPI_CR1_MSTR;              	// MCU is SPI controller
   // CR2 (reset value = 0x0700 : 8b data)
   SPI1->CR2 &= ~( SPI_CR2_TXEIE | SPI_CR2_RXNEIE ); // disable FIFO intrpts
   SPI1->CR2 &= ~( SPI_CR2_FRF);              	// Moto frame format
   SPI1->CR2 |=	 SPI_CR2_NSSP;              	// auto-generate NSS pulse
   SPI1->CR2 |=	 SPI_CR2_DS;                	// 16-bit data
   SPI1->CR2 |=	 SPI_CR2_SSOE;              	// enable SS output// CR1
   SPI1->CR1 |=	 SPI_CR1_SPE;               	// re-enable SPI for ops
}

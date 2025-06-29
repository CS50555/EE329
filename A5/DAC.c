/*******************************************************************************
 * EE 329 A5 DAC/SPI FUNCTIONS
 *******************************************************************************
 * @file           : DAC.c
 * @brief          : SPI and DAC configuration
 * project         : EE 329 S'25 - Assignment A5
 * authors         : Kelvin Shi, Brayden Daly
 * version         : 1.0
 * date            : 2025/05/15
 * compiler        : STM32CubeIDE v.1.12.0 Build: 14980_20230301_1550 (UTC)
 * target          : NUCLEO-L4A6ZG
 * clocks          : 4 MHz MSI to AHB2
 * @attention      : (c) 2023 STMicroelectronics.  All rights reserved.
 *******************************************************************************
 */

#include "DAC.h"

/* -----------------------------------------------------------------------------
 * function : void DAC_init(void)
 * INs      : none
 * OUTs     : void
 * action   : Initialize the SPI and DAC GPIO pins; wake up DAC device
 * authors  : Kelvin Shi, Brayden Daly
 * version  : 1.0
 * date     : 2025/05/15
 * -------------------------------------------------------------------------- */
void DAC_init(){
	SPI_GPIO_setup();
	DAC_GPIO_setup();
	SPI_init();
	GPIOA->BSRR = (GPIO_PIN_1 | GPIO_PIN_0); // no shutdown
}


/* -----------------------------------------------------------------------------
 * function : uint16_t DAC_volt_conv(uint16_t)
 * INs      : uint16_t - the voltage to send to the DAC in mV
 * OUTs     : uint16_t - the 12 bit word to be sent to the DAC
 * action   : Convert the user defined input voltage to the correct level for
 * 			  the DAC to process
 * authors  : Kelvin Shi, Brayden Daly
 * version  : 1.0
 * date     : 2025/05/15
 * -------------------------------------------------------------------------- */
uint16_t DAC_volt_conv(uint16_t voltage){
	// cap the maximum voltage to the voltage rail
	if (voltage > VOLTAGE_RAIL){
		voltage = VOLTAGE_RAIL;
	}
	if (voltage < 0){
		voltage = 0;
	}

	// calculate the correct step
	float step = ((float) VOLTAGE_REF)/4095;
	return voltage/step;
}

/* -----------------------------------------------------------------------------
 * function : void DAC_write(uint16_t)
 * INs      : uint16_t - the 12 bit word to send to the DAC
 * OUTs     : void
 * action   : Using the SPI, send over the 12 bit data to the DAC and adjust for
 * 			  the gain/reference voltage threshold
 * authors  : Kelvin Shi, Brayden Daly
 * version  : 1.0
 * date     : 2025/05/15
 * -------------------------------------------------------------------------- */
void DAC_write(uint16_t data){
	GPIOA->BSRR = GPIO_PIN_0;
	uint16_t command;
	// check if the data is over the internal reference voltage
	if (data > 4095U){
		// adjust for double gain
		command = CONTROL_BITS_G2;
		data /= 2;
	}
	else
		command = CONTROL_BITS_G1;

	// only mask the last 12 bits
	command |= (data & 0x0FFF);
	// ensure that the transmission buffer is cleared before sending
	while (!(SPI1->SR & 0x02));
	SPI1->DR = command;
}

/* -----------------------------------------------------------------------------
 * function : void SPI_init(void)
 * INs      : void
 * OUTs     : void
 * action   : Enable the SPI peripheral and configure it for communication
 * authors  : Kelvin Shi, Brayden Daly
 * version  : 1.0
 * date     : 2025/05/15
 * -------------------------------------------------------------------------- */
void SPI_init( void ) {
   // SPI config as specified @ STM32L4 RM0351 rev.9 p.1459
   // called by or with DAC_init()
   // build control registers CR1 & CR2 for SPI control of peripheral DAC
   // assumes no active SPI xmits & no recv data in process (BSY=0)
   // CR1 (reset value = 0x0000)
   SPI1->CR1 &= ~( SPI_CR1_SPE );             	// disable SPI for config
   SPI1->CR1 &= ~( SPI_CR1_RXONLY );     // recv-only OFF (MOSI/MISO active)
   SPI1->CR1 &= ~( SPI_CR1_LSBFIRST );        	// data bit order MSb:LSb
   SPI1->CR1 &= ~( SPI_CR1_CPOL | SPI_CR1_CPHA ); // SCLK polarity:phase = 0:0
   SPI1->CR1 |=	 SPI_CR1_MSTR;              	// MCU is SPI controller

   // CR2 (reset value = 0x0700 : 8b data)
   SPI1->CR2 &= ~( SPI_CR2_TXEIE | SPI_CR2_RXNEIE ); // disable FIFO intrpts
   SPI1->CR2 &= ~( SPI_CR2_FRF);              	// Moto frame format
   SPI1->CR2 |=	 SPI_CR2_NSSP;              	// auto-generate NSS pulse
   SPI1->CR2 |=	 SPI_CR2_DS;                	// 16-bit data
   SPI1->CR2 |=	 SPI_CR2_SSOE;              	// enable SS output
   // CR1 enable
   SPI1->CR1 |=	 SPI_CR1_SPE;               	// re-enable SPI for ops
}

/* -----------------------------------------------------------------------------
 * function : void SPI_GPIO_setup(void)
 * INs      : void
 * OUTs     : void
 * action   : Configure the GPIOA ports for SPI alternate function use.
 * 			  NOTE: does not use POCI since DAC is simplex communication
 * authors  : Kelvin Shi, Brayden Daly
 * version  : 1.0
 * date     : 2025/05/15
 * -------------------------------------------------------------------------- */
void SPI_GPIO_setup(){
	// enable clock for GPIOA & SPI1
	RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOAEN);          // GPIOA: DAC NSS/SCK/SDO
	RCC->APB2ENR |= (RCC_APB2ENR_SPI1EN);           // SPI1 port

	// Configure SPI GPIO pins as AF mode, push-pull, and high speed
	GPIOA->MODER &= ~(GPIO_MODER_MODE4 | GPIO_MODER_MODE5 | GPIO_MODER_MODE7);
	GPIOA->MODER |= ((2 << 8) | (2 << 10) | (2 << 14));
	GPIOA->OTYPER &= ~(GPIO_OTYPER_OT4 | GPIO_OTYPER_OT5 | GPIO_OTYPER_OT7);
	GPIOA->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEED4_Pos | GPIO_OSPEEDR_OSPEED5_Pos
			| GPIO_OSPEEDR_OSPEED7_Pos);
	GPIOA->OSPEEDR |= ((3 << GPIO_OSPEEDR_OSPEED4_Pos)
			| (3 << GPIO_OSPEEDR_OSPEED5_Pos) | (3 << GPIO_OSPEEDR_OSPEED7_Pos));

	// SPI NSS AF configuration
	GPIOA->AFR[0] &= ~((0x000F << GPIO_AFRL_AFSEL4_Pos));
	GPIOA->AFR[0] |=  ((0x0005 << GPIO_AFRL_AFSEL4_Pos));

	// SPI SCK AF configuration
	GPIOA->AFR[0] &= ~((0x000F << GPIO_AFRL_AFSEL5_Pos));
	GPIOA->AFR[0] |=  ((0x0005 << GPIO_AFRL_AFSEL5_Pos));

	// SPI MOSI AF configuration
	GPIOA->AFR[0] &= ~((0x000F << GPIO_AFRL_AFSEL7_Pos));
	GPIOA->AFR[0] |=  ((0x0005 << GPIO_AFRL_AFSEL7_Pos));
}

/* -----------------------------------------------------------------------------
 * function : void DAC_GPIO_setup(void)
 * INs      : void
 * OUTs     : void
 * action   : Configures the GPIP pins that control the DAC's "SHUTDOWN" and
 * 			  "LDAC"
 * authors  : Kelvin Shi, Brayden Daly
 * version  : 1.0
 * date     : 2025/05/15
 * -------------------------------------------------------------------------- */
void DAC_GPIO_setup(){

	// Configure GPIO has output mode, push-pull, no PUPD, high speed
	GPIOA->MODER &= ~(GPIO_MODER_MODE0 | GPIO_MODER_MODE1);
	GPIOA->MODER |= ((1 << 0) | (1 << 2));

	GPIOA->OTYPER &= ~(GPIO_OTYPER_OT0 | GPIO_OTYPER_OT1);

	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPD0 | GPIO_PUPDR_PUPD1);

	GPIOA->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEED0_Pos | GPIO_OSPEEDR_OSPEED1_Pos);
	GPIOA->OSPEEDR |= ((3 << GPIO_OSPEEDR_OSPEED0_Pos)
			| (3 << GPIO_OSPEEDR_OSPEED1_Pos));
}

/* -----------------------------------------------------------------------------
 * function : void DAC_update(void)
 * INs      : void
 * OUTs     : void
 * action   : Toggles the LDAC pin to update the DAC to reflect new input data
 * authors  : Kelvin Shi, Brayden Daly
 * version  : 1.0
 * date     : 2025/05/15
 * -------------------------------------------------------------------------- */
void DAC_update(){
	// toggle the LDAC pin with a small software delay
	GPIOA->BRR = GPIO_PIN_0;
	for (int i = 0; i < 5; i++);
	GPIOA->BSRR = GPIO_PIN_0;
}

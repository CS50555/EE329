/*
 * UART.c
 *
 *  Created on: May 27, 2025
 *      Author: bdaly
 */


/*
 * UART.c
 *
 *  Created on: May 7, 2025
 *      Author: bdaly, Zachary Lee
 */

#include <main.h>
#include <UART.h>

//global variables to hold the position and start game interrupt
uint32_t x = 0;
uint32_t y = 0;
#define MOHMS 10000 // 10 Ohms RE
/* -----------------------------------------------------------------------------
 * function : LPUART_init
 * INs      : none
 * OUTs     : none
 * action   : initializes UART GPIO (PORT G 7,8), sets baud rate
 *
 * authors  : Brayden Daly, Zachary Lee
 * version  : 0.3
 * date     : 250507
 * -------------------------------------------------------------------------- */
void LPUART_init(void)
{
	PWR->CR2 |= (PWR_CR2_IOSV);  // power avail on PG[15:2] (LPUART1)
	RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOGEN);  // enable UART_PORT clock
	RCC->APB1ENR2 |= RCC_APB1ENR2_LPUART1EN; // enable LPUART clock bridge
	/* USER: configure UART_PORT registers MODER/PUPDR/OTYPER/OSPEEDR then
	 select AF mode and specify which function with AFR[0] and AFR[1] */

	//RECEIVER (RX) PG8
	UART_PORT->MODER &= ~(GPIO_MODER_MODE7 | GPIO_MODER_MODE8);
	//TRANSMITTER (TX) PG7
	UART_PORT->MODER |= (GPIO_MODER_MODE7_1 | GPIO_MODER_MODE8_1); // Set AF mode (10)
	UART_PORT->OTYPER &= ~(GPIO_OTYPER_OT7);
	//pullups
	//UART_PORT->PUPDR |= (GPIO_PUPDR_PUPD7_0 | GPIO_PUPDR_PUPD8_0); // Enable pull-up for PG7 and PG8
	UART_PORT->PUPDR &= 0; // Enable pull-up for PG7 and PG8
	// Set very high output speed for PC7, 8
	UART_PORT->OSPEEDR |=  ((3 << GPIO_OSPEEDR_OSPEED7_Pos) |
					 (3 << GPIO_OSPEEDR_OSPEED8_Pos));
	UART_PORT->BRR = (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3); // preset PC0, PC1, PC2, PC3 to 0


	UART_PORT->AFR[0] &= ~(0x000F << GPIO_AFRL_AFSEL7_Pos); // clear PG7 nibble AF
	UART_PORT->AFR[0] |= (0x0008 << GPIO_AFRL_AFSEL7_Pos); // set PG7 AF = LPUART1_TX
	UART_PORT->AFR[1] &= ~(0x000F << GPIO_AFRH_AFSEL8_Pos); // clear PG8 nibble AF
	UART_PORT->AFR[1] |= (0x0008 << GPIO_AFRH_AFSEL8_Pos); // set PG8 AF = LPUART1_RX

	LPUART1->CR1 &= ~(USART_CR1_M1 | USART_CR1_M0); // 8-bit data
	LPUART1->CR1 |= USART_CR1_UE;                   // enable LPUART1
	LPUART1->CR1 |= (USART_CR1_TE | USART_CR1_RE);  // enable xmit & recv
	LPUART1->CR1 |= USART_CR1_RXNEIE;        // enable LPUART1 recv interrupt
	LPUART1->ISR &= ~(USART_ISR_RXNE);       // clear Recv-Not-Empty flag
	/* USER: set baud rate register (LPUART1->BRR) */
	LPUART1->BRR = 0x22B9; //8889
	NVIC->ISER[2] = (1 << (LPUART1_IRQn & 0x1F));   // enable LPUART1 ISR
	__enable_irq();

	//clear screen and move cursor to top left
	LPUART_Print( "\x1b[2J");
	LPUART_Print( "\x1b[H");
}


/* -----------------------------------------------------------------------------
 * function : LPUART_Print
 * INs      : string
 * OUTs     : none
 * action   : print a string to the screen
 *
 * authors  : Brayden Daly, Zachary Lee
 * version  : 0.3
 * date     : 250507
 * -------------------------------------------------------------------------- */
void LPUART_Print( const char* message ) {
   uint16_t iStrIdx = 0;
   while ( message[iStrIdx] != 0 ) {
      while(!(LPUART1->ISR & USART_ISR_TXE)) // wait for empty xmit buffer
         ;
      LPUART1->TDR = message[iStrIdx];       // send this character
	iStrIdx++;                             // advance index to next char
   }
}

/* -----------------------------------------------------------------------------
 * function : LPUART_IRQHandler
 * INs      : none
 * OUTs     : none
 * action   : Interrupt Handler to check for ESC key presses
 *
 * authors  : Brayden Daly, Zachary Lee
 * version  : 0.3
 * date     : 250507
 * -------------------------------------------------------------------------- */
void LPUART1_IRQHandler( void  ) {
	//set variable to hold which character was received
   uint8_t charRecv;
   //check if there was an interrupt
   if (LPUART1->ISR & USART_ISR_RXNE) {
	   //set charRecv as the input from the UART interrupt
      charRecv = LPUART1->RDR;
      //switch to character inputted
      switch ( charRecv ) {
      //If R pressed, make font color red
	   case 'R':
		   LPUART_Print( "\x1b[31m");
	      break;

	   	//for default, wait for TX buffer
	   default:
	      while( !(LPUART1->ISR & USART_ISR_TXE) )
	      {
               ;    // wait for empty TX buffer
	      }
	      //reset start game

		LPUART1->TDR = charRecv;  // echo char to terminal
	}  // end switch
   }
}


/* -----------------------------------------------------------------------------
 * function : print_uint4
 * INs      : uint16_t
 * OUTs     : none
 * action   : print the value as a uint8_t
 *
 * authors  : Brayden Daly, Preston
 * version  : 0.3
 * date     : 250507
 * -------------------------------------------------------------------------- */
void print_uint4(uint16_t val) {
	//initialize array and store the values
	char out[5];
	out[0] = '0' + (val / 1000) % 10;
	out[1] = '0' + (val / 100) % 10;
	out[2] = '0' + (val / 10) % 10;
	out[3] = '0' + (val % 10);
	out[4] = '\0';

	//print the values to terminal
	LPUART_Print((uint8_t*)out);
}

/* -----------------------------------------------------------------------------
 * function : calibrate_voltage
 * INs      : none
 * OUTs     : none
 * action   : use line of best fit to calibrate the adc
 *
 * authors  : Brayden Daly, Preston
 * version  : 0.3
 * date     : 250507
 * -------------------------------------------------------------------------- */
//calibration, V = 0.000805x + 0.00362
uint32_t calibrate_voltage(uint16_t adc_counts) {
	return (805 * (uint32_t) adc_counts + 3620) / 1000;
}


/* -----------------------------------------------------------------------------
 * function : print voltage
 * INs      : none
 * OUTs     : none
 * action   : print the voltage in volts
 *
 * authors  : Brayden Daly, Preston
 * version  : 0.3
 * date     : 250507
 * -------------------------------------------------------------------------- */
void print_voltage(uint16_t adc_val) {
	//calibrate the voltage
	uint32_t mV = calibrate_voltage(adc_val);
	//initialze array and store the mv values in each index of array
	char out[10];
	out[0] = '0' + (mV / 1000);
	out[1] = '.';
	out[2] = '0' + (mV % 1000) / 100;
	out[3] = '0' + (mV % 100) / 10;
	out[4] = '0' + (mV % 10);
	out[5] = ' ';
	out[6] = 'V';
	out[7] = '\r';
	out[8] = '\n';
	out[9] = '\0';
	//print the voltage
	LPUART_Print((uint8_t*)out);
}

/* -----------------------------------------------------------------------------
 * function : print_current
 * INs      : none
 * OUTs     : none
 * action   : print the current in amps
 *
 * authors  : Brayden Daly, Preston
 * version  : 0.3
 * date     : 250507
 * -------------------------------------------------------------------------- */
void print_current(uint16_t adc_val) {
	//calibrate voltage
	uint32_t uV = calibrate_voltage(adc_val) * 1000;
	//divide by Re to get the current
	uint32_t uA = uV / MOHMS;
	//initialize array and store microamps in indices
	char out[12];
	out[0] = '0' + (uA / 1000);
	out[1] = '.';
	out[2] = '0' + (uA % 1000) / 100;
	out[3] = '0' + (uA % 100) / 10;
	out[4] = '0' + (uA % 10);
	out[5] = ' ';
	out[6] = 'A';
	out[7] = '\r';
	out[8] = '\n';
	out[9] = '\0';
	//print the coil current and value of coil current
	LPUART_Print((uint8_t*)"coil current = ");
	LPUART_Print((uint8_t*)out);
}


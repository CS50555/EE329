/* USER CODE BEGIN Header */
/*******************************************************************************
 * EE 329 A4 Interrupts & Timers
 *******************************************************************************
 * @file           : main.c
 * @brief          :
 * project         : EE 329 S'25 Assignment 4
 * authors         : Tyler Wong (TW) - twong103@calpoly.edu, Brayden Daly ;
 * version         : 0.1
 * date            : 250416
 * compiler        : STM32CubeIDE v.1.18.0
 * target          : NUCLEO-L4A6ZG
 * clocks          : 4 MHz MSI to AHB2
 * @attention      : (c) 2025 STMicroelectronics.  All rights reserved.
 *******************************************************************************
 * LED WIRING
 *      peripheral – Nucleo I/O
 * LED 0  PC0 = CN9  - 3
 * LED 1  PC1 = CN9  - 7
 * LED 2  PC2 = CN10 - 9
 * LED 3  PC3 = CN9  - 5
 *******************************************************************************
 * REVISION HISTORY
 * 0.1 230318 (TW) created, written
 *******************************************************************************
 * 45678-1-2345678-2-2345678-3-2345678-4-2345678-5-2345678-6-2345678-7-234567 */
/* USER CODE END Header */

#include "main.h"
#include "LCD.h"
#include "Delay.h"
#include "string.h"
#include "TIMER.h"
#include "stdio.h"

//declare functions below main
void SystemClock_Config(void);
uint32_t get_random_number(void);

//variables to hold strings and total time after flash
uint32_t time_elapse = 0;
char stringtime[30];
char fullstring[30];


/* -----------------------------------------------------------------------------
 * function : main
 * INs      : none
 * OUTs     : int
 * action   : runs game and writes to LCD
 *
 * authors  : Brayden Daly, Tyler Wong
 * version  : 0.3
 * date     : 253004
 * -------------------------------------------------------------------------- */
int main(void) {
	HAL_Init();
	SystemClock_Config();

	//enable clock for LED
	RCC->AHB2ENR   |=  (RCC_AHB2ENR_GPIOCEN);
	// reset output mode
	LED_GPIO->MODER   &= ~(GPIO_MODER_MODE7);
	// set mode to output (set PC13 to Input)
	LED_GPIO->MODER   |=  (GPIO_MODER_MODE7_0);
	//
	LED_GPIO->OTYPER  &= ~(GPIO_OTYPER_OT7);
	// set all pins to no pull up or pull down
	LED_GPIO->PUPDR   &= ~(GPIO_PUPDR_PUPD7);
	// set all pin speeds to high
	LED_GPIO->OSPEEDR |=  ((3 << GPIO_OSPEEDR_OSPEED7_Pos));
	// reset all pins to 0
	LED_GPIO->BRR = (GPIO_PIN_7);
	//set pin 13 to input mode
	GPIOC->MODER &= ~(GPIO_MODER_MODE13);  // sets to input mode

	//Enable Random number generating clock
	RCC->AHB2ENR   |=  (RCC_AHB2ENR_RNGEN);
	//Enable HSI48 clock
	RCC->CRRCR |= RCC_CRRCR_HSI48ON;
	//wait until the HSI48 clock is stable
	while((RCC->CRRCR & RCC_CRRCR_HSI48RDY)==0);
	//clear the clock select bits
	RCC->CCIPR &= ~RCC_CCIPR_CLK48SEL;
	//select HSI48 as CLK38 source
	RCC->CCIPR |= (0b11 << RCC_CCIPR_CLK48SEL_Pos);
	//Enable the RNG peripheral
	RNG->CR |= RNG_CR_RNGEN;

	//initialize Systick for delay_us
	 SysTick_Init();
	 //configure the LCD
	 LCD_config();
	 //Initialize the LCD
	 LCD_Init();
	 //clears the LCD
	 LCD_command(0x01);
	 //wait 2000 seconds
	 delay_us(2000);
	 //set the cursor to first line first index
	 LCD_Set_Cursor(0,0);
	 //write this string to the first line
	 LCD_write_string("EE 329 A4 REACT ");
	 //set the cursor to first line
	 LCD_Set_Cursor(1,0);
	 //write this string to the second line
	 LCD_write_string("PUSH SW TO TRIG ");
	 //turns on display and sets cursor
	 LCD_command(0x0F);
	 //set the GPIOC to input mode for PC13
	 GPIOC->MODER &= ~(GPIO_MODER_MODE13);

	 //initialize and declare statenum and randomint to hold values
	 //for the state
	 // and random integer returned
 	 uint8_t statenum = 0;
 	 uint32_t randomint = 0;
 	 //loop forever to run game forever
 	 while (1)
 	 {
		 switch (statenum)
		 {
		 	 //first state -> write title screen and delay until
		 	 //button is pressed
			 case 0:
				 //turn off LED
				 LED_GPIO->BRR = (GPIO_PIN_7);
				 //Clear LCD
				 LCD_Clear();
				 //Set cursor to first index first line
				 LCD_Set_Cursor(0,0);
				 //write string to line 1
				 LCD_write_string("EE 329 A4 REACT ");
				 //set cursor line 1 first index
				 LCD_Set_Cursor(1,0);
				 //write string
				 LCD_write_string("PUSH SW TO TRIG ");
				 //delay for unwanted bounce
				 delay_us(1000000);
				 //wait until button pressed
				while ((GPIOC->IDR & GPIO_IDR_ID13) == 0)
				{
						  continue;
				}
				//increment state num (go to next state)
				statenum ++;
				//break out of switch statement
				break;
			//state 2 -> generate random integer
			 case 1:
				 	 //generate random int and store into
				 	 //variable
					 randomint = get_random_number();
					 //go to next state
					 statenum ++;
					 //break out of switch statment
					 break;
			//stage 3 -> turn on LED and wait for reaction
			 case 2:
				 	 //delay for random amount of time
					 delay_us(randomint);
					 //turn on led
					 LED_GPIO->BSRR = (GPIO_PIN_7);
					 //start ARR timer
					 setup_TIM2(100);
					 // wait for button press
				 	 while (((GPIOC->IDR & GPIO_IDR_ID13) == 0))
					 {
				 		 //if button not pressed within
				 		 //10 sec, reset
				 		 if (time_elapse >= 10000)
						 {
				 			 //set statenum to -1 since it auto
				 			 //accum at end
							 statenum = -1;
							 //restart timer
							 time_elapse = 0;
							 //break out of while loop
							 break;
				 		  }
					 }


				 	 //turn off all leds
				 	LED_GPIO->BRR = (GPIO_PIN_7);
				 	// go to next state
				 	//(unless 10 seconds violation)
					 statenum ++;
					 //break out of switch
					 break;
			//stage 4 -> calculate the seconds and write to LCD
			 case 3:
				 //calculate milliseconds into seconds
					 uint32_t seconds = time_elapse / 1000;
					 uint32_t millis_remainder = time_elapse % 1000;
					 //turn seconds into a string to write
					 sprintf(fullstring, "TIME: %lu.%03lu",
							 seconds, millis_remainder);
					 //disable the interrupt
					  __disable_irq();
					 //clear the LCD
					 LCD_Clear();
					 //Set the cursor
					 LCD_Set_Cursor(1,2);
					 //write the seconds to the LCD
					 LCD_write_string(fullstring);
					 //go to next state
					 statenum ++;
					 //break out of switch statement
					 break;
			//stage 5 -> wait for button press to restart
			 case 4:
				 //small delay for debounce
				 	delay_us(1000000);
				 	//wait for button press
				 	while ((GPIOC->IDR & GPIO_IDR_ID13) == 0)
				 	{
				 		continue;
				 	}
				 	//if button pressed,reset the timer
				 	//and state number
				 	time_elapse = 0;
				 	statenum = 0;
				 	//break out of switch statment
				 	break;
		 }
 	 }


}




/* ---------------------------------------------------------------------------*/
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}


void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}


/* -----------------------------------------------------------------------------
 * function : get_random_number
 * INs      : none
 * OUTs     : uintn32_t
 * action   : calculates a random number and returns it
 *
 * authors  : Brayden Daly, Tyler Wong
 * version  : 0.3
 * date     : 253004
 * -------------------------------------------------------------------------- */
uint32_t get_random_number(void) {
	// Wait until data is ready
   while ((RNG->SR & RNG_SR_DRDY) == 0);
   // Read 32-bit random value
   return RNG->DR;
}


#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

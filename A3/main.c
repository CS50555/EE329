/* USER CODE BEGIN Header */
/*******************************************************************************
 * EE 329 A3 <Name>
 *******************************************************************************
 * @file           : main.c
 * @brief          : <Description>
 * project         : EE 329 S'25 Assignment 3
 * authors         : Michael Brandon Lee, Brayden Daly
 * target          : NUCLEO-L4A6ZG
 * @attention      : (c) 2023 STMicroelectronics.  All rights reserved.
 *******************************************************************************
 * LCD Plan :
 *
 *******************************************************************************
 * LCD Wiring 4 DATA PINS 1 ENABLE 1 READ/WRITE 1 REG SELECT
 * NOTE: 15 pins total pin1 will be ground and pin 15 will be the backlight neg
 *      peripheral – Nucleo I/O
 * LCD 1
 *******************************************************************************
 * REVISION HISTORY
 * 0.1 4/18/25	Formated code headers and imported keypad files
 *******************************************************************************
 * TODO
 *
 ******************************************************************************/
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "keypad.h"
#include "lcd.h"
#include "string.h"
#include "stdio.h"
#include "LED.h"

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */


/* -----------------------------------------------------------------------------
 * function : main( )
 * INs      : none
 * OUTs     : int
 * action   : initialize LCD, Keypad, and LED, run timer on LCD, take I/O from
 * keypad
 * authors  : Michael Brandon Lee, Brayden Daly
 * -------------------------------------------------------------------------- */
int main(void) {
	HAL_Init();
	SystemClock_Config();

	// initilize delay module
	SysTick_Init();
	// initilize LCD GPIO pins and send init commands
	LCD_Setup();
	LCD_Init();
	// initilize Keypad gpio pins
	Keypad_Config();
	// initilize gpio pins for LED
	LED_Config();

	//initialize timer for LCD with string
	char time_string[] = "__:__";
	//Variables to hold minutes and seconds values
	int minutes = 0, seconds = 0;
	//Variables to hold values for reset, inputs, key, and values
	int8_t reset = 0, input = 0xff, update_input = 0, last_value = 0xff,
			value = 0xff, key = 0xff, input_char = ' ';
	int32_t tot_sec = 0, consec_reads = 0;

	// print string to line 1
	LCD_Command(LCD_LINE1);
	//print first line to lcd
	LCD_Print_String( "EE 329 A3 TIMER " );
	// print string to line 2
	LCD_Command(LCD_LINE2);
	LCD_Print_String( "*=SET #=GO __:__" );

	// wait for * input
	while (1) {
		//check if key is pressed and store result in input
		input = keypad_WaitForPress();
		//if * is pressed, break out of loop
		if ( input == 0xe )
			break;
	}

	//constantly change lcd to update timer
	while (1) {
		strcpy(time_string, "__:__");
		//move cursor to lcd
		LCD_Command( MOVE_CURSOR(2, 11) );
		LCD_Print_String(time_string);
		//unflag reset
		reset = 0;

		// set cursor to begining xx:xx and turn on cursor blink
		LCD_Command( MOVE_CURSOR(2, 11) );
		LCD_Command ( LCD_DISPLAY_CTRL(1, 0, 1) );

		// input for loop
		for (uint8_t i = 0; i < 5; i++) {
			// skip ':' position
			if (i == 2) {
				LCD_Command(SHIFT_CURSOR_RIGHT);
				continue;
			}
			else {
				while (1){
					input = keypad_WaitForPress();
					// break out of while if valid key detected
					if ( (input < 10) || (input == 0xe) )
						break;
				}
				// break out of input loop if *
				if ( input == 0xe ) {
					reset = 1;
					break;
				}
				// update time_string and write character to display
				else {
					input_char = keypad_char_table[input];
					time_string[i] = input_char;
					LCD_Write_Char( input_char );
					// wait for key release to loop
					keypad_WaitForRelease();
				}
			}
		}

		//print the
		LCD_Command ( LCD_DISPLAY_CTRL(1, 0, 0) );
		sscanf(time_string, "%2d:%2d", &minutes, &seconds);

		//if minutes or seconds greater than 59, update both to 59
		//(make sure cannot have value greater than 59)
		if ( minutes > 59 ) {
			minutes = 59;
			update_input = 1;
		}
		if ( seconds > 59 ) {
			seconds = 59;
			update_input = 1;
		}

		//if there is a number larger than 59, update to 59
		//and print to LCD
		if (update_input) {
			LCD_Command( MOVE_CURSOR(2, 11) );
			sprintf(time_string, "%02d:%02d", minutes, seconds);
			LCD_Print_String(time_string);
		}

		//calculate the minutes and seconds into seconds
		tot_sec = (minutes * 60) + seconds;
		//loop and check if keypad is pressed
		//if # or * pressed, break out of loop
		while(1) {
			input = keypad_WaitForPress();
			// break out of while if valid key detected
			if ( (input == 0xf) || (input == 0xe) )
				break;
		}
		//if # pressed loop every second and count down timer
		if ( input == 0xf ) {
			// for loops every second
			for ( ; tot_sec >= 0; tot_sec-- ) {
				// update lcd with minutes and seconds
				minutes = tot_sec / 60;
				seconds = tot_sec % 60;
				//print minutes and seconds to lcd
				LCD_Command( MOVE_CURSOR(2, 11) );
				sprintf(time_string, "%02d:%02d", minutes, seconds);
				LCD_Print_String(time_string);

				// loops every 50ms
				//every 50 ms, check if reset key is pressed
				for ( uint8_t loop_msec = 0; loop_msec < 20; loop_msec++) {
					delay_us(47750);
					if( keypad_IsAnyKeyPressed() ) {
						// update last value before overwriting value
						last_value = value;
						value = 1;
						// check if value is the same as the last value
						//debounce for reset key
						if (value == last_value) {
							if (consec_reads < 200)
								consec_reads++;
						}
						else
							consec_reads = 0;
						//if 4 consecutive reads detected,
						//check if it is the reset key then
						//break if it is the reset key
						if ( consec_reads == 4 ) {
							key = keypad_WhichKeyPressed();
							if ( key == 0xe ) {
								reset = 1;
								break;
							}
						}
					}
					else {
						// update last value before overwriting value
						last_value = value;
						value = 0;
						//if value equals last value
						//subtract one from consecutive reads
						if (value == last_value) {
							if (consec_reads > -200)
								consec_reads--;
						}
						else
							consec_reads = 0;
					}
				}
				//break out of loop if reset is caught
				if ( reset == 1 )
					break;
			}
//			LCD_Command( MOVE_CURSOR(2, 11) );
//			sprintf(time_string, "%02d:%02d", minutes, seconds);
//			LCD_Print_String(time_string);
			// turn on LED
			GPIOC->BSRR = GPIO_PIN_0;
			// wait until * or # are pressed
			while(1) {
				input = keypad_WaitForPress();
				if ( input == 0xf )
					GPIOC->BRR = GPIO_PIN_0;
				else if ( input == 0xe ) {
					GPIOC->BRR = GPIO_PIN_0;
					reset = 1;
					break;
				}
			}
		}
		else
			reset = 1;
	}
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	/** Configure the main internal regulator output voltage
	 */
	if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1)
			!= HAL_OK) {
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
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) {
		Error_Handler();
	}
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
	}
	/* USER CODE END Error_Handler_Debug */
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

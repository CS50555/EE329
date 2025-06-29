/*******************************************************************************
* EE 329 A5 USER DEFINED DAC
********************************************************************************
* @file           : main.c
* @brief          : Covnert the user input to an actual voltage via the DAC
* project         : EE 329 S'25 - Assignment A5
* authors         : Kelvin Shi , Brayden Daly
* version         : 1.0
* date            : 2025/05/15
* compiler        : STM32CubeIDE v.1.12.0 Build: 14980_20230301_1550 (UTC)
* target          : NUCLEO-L4A6ZG
* clocks          : 4 MHz MSI to AHB2
* @attention      : (c) 2023 STMicroelectronics.  All rights reserved.
*******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "DAC.h"
#include "keypad.h"

/* -----------------------------------------------------------------------------
 * function : int main(void)
 * INs      : void
 * OUTs     : int - status value
 * action   : The entry point of the program
 * authors  : Kelvin Shi , Brayden Daly
 * version  : 1.0
 * date     : 2025/05/15
 * -------------------------------------------------------------------------- */
int main(void){
  // Reset of all peripherals, Initializes the Flash interface and the Systick.
  HAL_Init();
  /* Configure the system clock */
  SystemClock_Config();
  DAC_init();
  keypad_Config();

  uint16_t value;
  /* Infinite loop */
  while (1){
	  value = getUserInput();
	  DAC_write(DAC_volt_conv(value));
	  DAC_update();
  }
}

/* -----------------------------------------------------------------------------
 * function : uint16_t getUserInput(void)
 * INs      : void
 * OUTs     : uint16_t - returns the user input from a keypad to a 16 bit word
 * action   : Enable the SPI peripheral and configure it for communication
 * authors  : Kelvin Shi , Brayden Daly
 * version  : 1.0
 * date     : 2025/05/15
 * -------------------------------------------------------------------------- */
uint16_t getUserInput(){
	uint8_t first, second, third;
	// get first valid digit
	do{
		first = keypad_getInput();
		if (first == 0xE){
			// restart process
			return getUserInput();
		}
	}
	while(first > 0x9);

	// get second valid digit
	do{
		second = keypad_getInput();
		if (second == 0xE){
			// restart process
			return getUserInput();
		}
	}
	while(second > 0x9);

	// get third valid digit
	do{
		third = keypad_getInput();
		if (third == 0xE){
			// restart process
			return getUserInput();
		}
	}
	while(third > 0x9);

	// computations of voltage in mV
	float voltage;
	voltage = first*1000 + second*100 + third*10;
	if (voltage > VOLTAGE_RAIL){
		voltage = VOLTAGE_RAIL;
	}

	return voltage;
}


/**
  * @brief System Clock Configuration
  * @retval None
  */
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
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

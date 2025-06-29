/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <DELAY.h>
#include <UART.h>
#include <ADC.h>

void SystemClock_Config(void);

#define MILLIOHMS 10000 // 10 ohms in milliohms

/* -----------------------------------------------------------------------------
 * function : main
 * INs      : none
 * OUTs     : none
 * action   : display voltage and currents read from DMM
 *
 * authors  : Brayden Daly, Preston
 * version  : 0.3
 * date     : 250507
 * -------------------------------------------------------------------------- */
int main(void)
{

	//initialze variables for later use
	uint8_t idx = 0;
	uint8_t fullarr = 0;
	uint32_t arraysum = 0;
	uint8_t psbw;

  HAL_Init();
  SystemClock_Config();

  //create array to hold adc_result
  uint16_t adc_result[32];

  // Initialize software delay, UART, and ADC
  SysTick_Init();
  LPUART_init();
  LPUART_Print("\x1b[2J"); // Clear terminal screen
  ADC_Init();


  psbw = (GPIOC->IDR & GPIO_PIN_13); // Read initial button state
  while (1) {
	  LPUART_Print("\x1b[2J"); // Clear terminal screen
	  LPUART_Print("\x1b[H"); //Set cursor upper left
	  // Relay control via USER button on PC13 (active-low logic)
	  if ((GPIOC->IDR & GPIO_PIN_13) == 0) {
		  GPIOB->BRR = GPIO_PIN_5; // De-energize relay
	  }
	  else
	  {
		  GPIOB->BSRR = GPIO_PIN_5; // Energize relay
	  }
	  // Process ADC sample when conversion is complete
	  if (adc_ready) {
		  adc_ready = 0; // Reset flag set by ADC interrupt
		  // Store sample and update circular buffer idx
		  adc_result[idx++] = adc_measurement;
		  if (idx >= 20) {
			  idx = 0;
			  fullarr = 1;
		  }
		  // compute values if array is full
		  if (fullarr) {
			  //set the sum to 0 and max low and min high
			  uint32_t arraysum = 0;
			  uint32_t min = 0xFFFF;
			  uint32_t max = 0;
			  //iterate through array and update mins and maxes
			  for (int i = 0; i < 20; i++) {
				  arraysum += adc_result[i];
				  if (adc_result[i] < min)
				  {
					  min = adc_result[i];

				  }
				  if (adc_result[i] > max)
				  {
					  max = adc_result[i];

				  }
			  }
			  //calculate average of array
				  uint16_t avg = arraysum / 20;
				  // Display values in terminal
				  LPUART_Print((char *)"ADC counts volts\r\n");
				  //print the min voltage
				  LPUART_Print((char *)"MIN "); print_uint4(min); LPUART_Print((char *)" ");
				  print_voltage(min);
				  //print the max voltage
				  LPUART_Print((char *)"MAX "); print_uint4(max); LPUART_Print((char *)" ");
				  print_voltage(max);
				  //print the average voltage
				  LPUART_Print((uint8_t*)"AVG "); print_uint4(avg); LPUART_Print((char *)" ");
				  print_voltage(avg);
				  print_current(avg); // Display estimated coil current
				  //small delay for readability
				  delay_us(100000);
			  }
			  ADC1->CR |= ADC_CR_ADSTART; // Start next ADC conversion
		}
	}
  return 0;
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

#ifdef  USE_fullarr_ASSERT
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
#endif /* USE_fullarr_ASSERT */

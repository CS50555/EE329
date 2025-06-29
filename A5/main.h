/*
 *******************************************************************************
 * EE 329 A5 MAIN SUPPORT
 *******************************************************************************
 * @file           : main.h
 * @brief          : main support functions and definitions
 * project         : EE 329 S'25 - Assignment A5
 * authors         : Kelvin Shi, Brayden Daly
 * version         : 1.0
 * date            : 2025/05/15
 * compiler        : STM32CubeIDE v.1.12.0 Build: 14980_20230301_1550 (UTC)
 * target          : NUCLEO-L4A6ZG
 * clocks          : 4 MHz MSI to AHB2
 * @attention      : (c) 2023 STMicroelectronics.  All rights reserved.
 *******************************************************************************
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);
uint16_t getUserInput(void);
void SystemClock_Config(void);


#ifdef __cplusplus
}
#endif
#endif /* __MAIN_H */

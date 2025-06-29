/*
 *******************************************************************************
 * EE 329 A5 SPI/DAC SUPPORT
 *******************************************************************************
 * @file           : DAC.h
 * @brief          : SPI/DAC support functions and definitions
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

#ifndef INC_SPI_H_
#define INC_SPI_H_
#endif /* INC_SPI_H_ */

#include "stm32l4xx_hal.h"

#define VOLTAGE_RAIL 3300
#define CONTROL_BITS_G1 0x3000
#define CONTROL_BITS_G2 0x1000
#define VOLTAGE_REF 2048

void SPI_GPIO_setup(void);
void SPI_init(void);
void DAC_init(void);
uint16_t DAC_volt_conv(uint16_t voltage);
void DAC_GPIO_setup(void);
void DAC_update(void);
void DAC_write(uint16_t data);



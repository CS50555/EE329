/*******************************************************************************
* EE 329 SPI Configuration - SPI Initialization Module
*******************************************************************************
* @file           : SPI_Config.c
* @brief          : Configures SPI1 and associated GPIO pins to communicate
*                   with external DAC. Sets up GPIO modes, alternate functions,
*                   and SPI control registers.
* project         : EE 329 S'25 Audio Playback Project
* authors         : Justin Rosu     (jrosu@calpoly.edu)
*                   Brayden Daly    (bdaly01@calpoly.edu)
*                   Alex Von Fuch   (avonfuch@calpoly.edu)
* version         : 1.0
* date            : 250606
* compiler        : STM32CubeIDE v.1.12.0 Build: 14980_20230301_1550 (UTC)
* target          : NUCLEO-L4A6ZG
* clocks          : 4 MHz MSI to AHB2
* @attention      : (c) 2025 STMicroelectronics. All rights reserved.
*******************************************************************************
* FUNCTIONAL OVERVIEW:
* - SPI_Config(): Initializes GPIOA for SPI1 alternate functions.
* - SPI_init(): Configures SPI1 control registers to send 16-bit frames to DAC.
*
* SPI1 Pin Assignments:
*   PA4 - NSS (manual control or NSSP)
*   PA5 - SCK
*   PA7 - MOSI (SDO)
*
* Note: SPI1 must be idle (BSY=0) before configuration.
*******************************************************************************
* REVISION HISTORY
* 1.0 250606 JR/B.D./A.V.F. Initial creation and SPI1 configuration
****
*/
#ifndef SRC_SPI_H_
#define SRC_SPI_H_

void SPI_init( void );
void SPI_Config(void);

#endif /* SRC_SPI_H_ */

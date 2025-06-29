/*******************************************************************************
* EE 329 Audio Playback System - Timer Header
*******************************************************************************
* @file           : TIMER.h
* @brief          : Function declarations for setting up TIM2 to control audio
*                   playback timing via periodic interrupts.
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
* - Exposes function to configure TIM2 for audio timing.
* - TIM2 interrupt triggers DAC mixing and playback advancement.
*******************************************************************************
* REVISION HISTORY
* 1.0 250606 JR/B.D./A.V.F. Initial creation and TIM2 configuration interface
*******************************************************************************
* 45678-1-2345678-2-2345678-3-2345678-4-2345678-5-2345678-6-2345678-7-2345678 */

#ifndef __TIMER_H
#define __TIMER_H

#include <stdint.h>

void Timer_setup_TIM2(int iDutyCycle);

// TIM2 IRQ handler is implemented in TIMER.c
void TIM2_IRQHandler(void);

#endif // __TIMER_H

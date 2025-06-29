/*******************************************************************************
* EE 329 Audio Playback System - Main Control
*******************************************************************************
* @file           : main.c
* @brief          : Initializes system peripherals and handles playback logic
*                   for multi-track audio with DAC output and button control.
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
* - Initializes HAL, DAC, timers, GPIOs, and buttons
* - Handles up to four concurrent audio sample playbacks
* - Selects playback mode via external GPIO inputs
* - Audio samples stored as uint16_t arrays in flash memory
* - Playback triggered by button press detection
*******************************************************************************
* REVISION HISTORY
* 1.0 250606 JR/B.D./A.V.F. Initial integration and multi-mode playback support
*******************************************************************************
* 45678-1-2345678-2-2345678-3-2345678-4-2345678-5-2345678-6-2345678-7-2345678 */


#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

extern uint16_t audio_sample, audio_sample2,audio_sample3,audio_sample4;
extern uint16_t audio_counter,audio_counter2,audio_counter3,audio_counter4;
extern uint8_t audio_playing,audio_playing2,audio_playing3,audio_playing4;
extern uint8_t which_audio_playing[];



void Error_Handler(void);

#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define LD3_Pin GPIO_PIN_14
#define LD3_GPIO_Port GPIOB
#define USB_OverCurrent_Pin GPIO_PIN_5
#define USB_OverCurrent_GPIO_Port GPIOG
#define USB_PowerSwitchOn_Pin GPIO_PIN_6
#define USB_PowerSwitchOn_GPIO_Port GPIOG
#define STLK_RX_Pin GPIO_PIN_7
#define STLK_RX_GPIO_Port GPIOG
#define STLK_TX_Pin GPIO_PIN_8
#define STLK_TX_GPIO_Port GPIOG
#define USB_SOF_Pin GPIO_PIN_8
#define USB_SOF_GPIO_Port GPIOA
#define USB_VBUS_Pin GPIO_PIN_9
#define USB_VBUS_GPIO_Port GPIOA
#define USB_ID_Pin GPIO_PIN_10
#define USB_ID_GPIO_Port GPIOA
#define USB_DM_Pin GPIO_PIN_11
#define USB_DM_GPIO_Port GPIOA
#define USB_DP_Pin GPIO_PIN_12
#define USB_DP_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define LD2_Pin GPIO_PIN_7
#define LD2_GPIO_Port GPIOB

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

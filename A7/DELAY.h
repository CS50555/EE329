/*
 * delay.h
 *
 *  Created on: Apr 21, 2025
 *      Author: bdaly
 */

#ifndef INC_DELAY_H_
#define INC_DELAY_H_

#include "stm32l4xx_hal.h"

void SysTick_Init( void );
void Delay_Us( const uint32_t time_us );

#endif /* INC_DELAY_H_ */

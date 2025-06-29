/*
 * TIMER.h
 *
 *  Created on: Apr 30, 2025
 *      Author: bdaly
 */

#ifndef TIMER_H_
#define TIMER_H_

#define PERIOD 4000

void setup_TIM2( int iDutyCycle );
void setup_MCO_CLK(void);
void TIM2_IRQHandler(void);




#endif /* INC_TIMER_H_ */

/*
 * ADC.h
 *
 *  Created on: May 27, 2025
 *      Author: bdaly, preston
 */

#ifndef ADC_H_
#define ADC_H_

// Global variables shared with main
extern volatile uint16_t adc_measurement;
extern volatile uint8_t adc_ready;
// Function prototypes
void ADC_Init(void);
void ADC1_2_IRQHandler(void);

#endif /* INC_ADC_H_ */

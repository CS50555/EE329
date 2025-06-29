/*
 * EEPROM.h
 *
 *  Created on: May 21, 2025
 *      Author: bdaly
 */

#ifndef EEPROM_H_
#define EEPROM_H_

#include "main.h"

#define I2CBUS I2C1			  //using I2C1
#define EEPROM_ADDR 0x55	  //using Address 0x55
#define TIMINGVAL 0x00E0257A  //Timing value for 4MHz Clock
#define I2CPORT GPIOB
#define SDA 9
#define SCK 8

void EEPROM_INIT(void);
uint8_t EEPROM_WriteByte(uint16_t address, uint16_t data);
uint8_t EEPROM_ReadByte(uint16_t address, uint16_t *data);

#endif /* INC_EEPROM_H_ */

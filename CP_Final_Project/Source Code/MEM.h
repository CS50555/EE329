/*
 * MEM.h
 *
 *  Created on: Jun 5, 2025
 *      Author: jrosu
 */

#ifndef MEM_H_
#define MEM_H_

#include "main.h"
#include "SPI.h"
#include "stm32l4xx.h"

#define CMD_WRITE_ENABLE   0x06
#define CMD_READ_STATUS1   0x05
#define CMD_PAGE_PROGRAM   0x02
#define CMD_READ_DATA      0x03
#define CMD_SECTOR_ERASE   0x20
#define CMD_RESET_ENABLE   0x66
#define CMD_RESET_DEVICE   0x99
#define CMD_SECTOR_ERASE 	0x20
#define CHIP_ERASE			0xC7
#define CS GPIOB


void GPIOB_Config(void);
void modify_sr1();
void modify_sr2();
void modify_sr3();
void global_unlock(void);
void reset(void);
void write_byte(uint32_t address, uint8_t data);
uint8_t read_byte(uint32_t addr);
void chip_erase(void);
void MEMCONFIG(uint32_t addr);



#endif /* INC_MEM_H_ */

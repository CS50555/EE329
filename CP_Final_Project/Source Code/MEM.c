//code that work

/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "SPI.h"
#include "stm32l4xx.h"
#include <MEM.h>

void GPIOB_Config(void)
{
	  //Initialize clock and inputs and outputs and stuff
	  //Enable GPIOC clock for peripheral
	  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;

	  //INITIALIZES COLUMNS AND OUTPUTS
	  CS->MODER &= ~(GPIO_MODER_MODE5|GPIO_MODER_MODE6 |GPIO_MODER_MODE8);
	  CS->MODER |=  (GPIO_MODER_MODE5_0|GPIO_MODER_MODE6_0);
	  CS->OSPEEDR |=  (3 << GPIO_OSPEEDR_OSPEED6_Pos);
	  CS->OTYPER &= ~(GPIO_OTYPER_OT6);
}

void write_enable(void)
{
    uint8_t cmd[1] = {0x06};
    GPIOB->BRR = GPIO_PIN_5;
    SPI1_Transmit(cmd, 1);
    GPIOB->BSRR = GPIO_PIN_5;
}


void wait_until_not_busy(void)
{
    uint8_t cmd[1] = {0x05};
    uint8_t status;
    do{// WIP bit is 1 = busy {
        GPIOB->BRR = GPIO_PIN_5;
        SPI1_Transmit(cmd, 1);
        status = SPI1_ReceiveByte();
        GPIOB->BSRR = GPIO_PIN_5;
    }while (status & 0x01);
}

void erase_sector(uint32_t address)
{
    uint8_t cmd[4] = {
        0x20, // Sector Erase (4KB)
        (address >> 16) & 0xFF,
        (address >> 8) & 0xFF,
        address & 0xFF
    };

    write_enable();

    GPIOB->BRR = GPIO_PIN_5;
    SPI1_Transmit(cmd, 4);
    GPIOB->BSRR = GPIO_PIN_5;

    wait_until_not_busy();
}

void modify_sr1()
{
    uint8_t cmd[2] = {0x01, 0x82}; // Write Status Register 1 = 0x00 (unprotect all blocks)

    write_enable(); // Send 0x06 to enable writing

    GPIOB->BRR = GPIO_PIN_5;           // CS LOW
    SPI1_Transmit(cmd, 2);            // Send 0x01 + 0x00
    GPIOB->BSRR = GPIO_PIN_5;         // CS HIGH
    wait_until_not_busy();

}
void modify_sr2()
{
    uint8_t cmd[2] = {0x31, 0x00}; // Write Status Register 1 = 0x00 (unprotect all blocks)

    write_enable(); // Send 0x06 to enable writing

    GPIOB->BRR = GPIO_PIN_5;           // CS LOW
    SPI1_Transmit(cmd, 2);            // Send 0x01 + 0x00
    GPIOB->BSRR = GPIO_PIN_5;         // CS HIGH
    wait_until_not_busy();

}
void modify_sr3()
{
    uint8_t cmd[2] = {0x11, 0x64}; // Write Status Register 1 = 0x00 (unprotect all blocks)

    write_enable(); // Send 0x06 to enable writing

    GPIOB->BRR = GPIO_PIN_5;           // CS LOW
    SPI1_Transmit(cmd, 2);            // Send 0x01 + 0x00
    GPIOB->BSRR = GPIO_PIN_5;         // CS HIGH
    wait_until_not_busy();

}
void global_unlock(void)
{

    uint8_t cmd[1] = {0x98};
    write_enable();
    GPIOB->BRR = GPIO_PIN_5;
    SPI1_Transmit(cmd, 1);
    GPIOB->BSRR = GPIO_PIN_5;
}

void reset(void)
{
    uint8_t cmd[1] = {0x66};
    GPIOB->BRR = GPIO_PIN_5;
    SPI1_Transmit(cmd, 1);

    GPIOB->BSRR = GPIO_PIN_5;
    GPIOB->BRR = GPIO_PIN_5;
    SPI1_Transmit(0x99, 1);
    GPIOB->BSRR = GPIO_PIN_5;
}

void test(void)
{
    uint8_t cmd[1] = {0x9F};
    GPIOB->BRR = GPIO_PIN_5;
    SPI1_Transmit(cmd, 1);
   uint8_t a =SPI1_ReceiveByte();
     a =SPI1_ReceiveByte();
   a = SPI1_ReceiveByte();
    GPIOB->BSRR = GPIO_PIN_5;
}


void write_byte(uint32_t address, uint8_t data) {
    uint8_t cmd[5] = {
        0x02, // Page Program
        (address >> 16) & 0xFF,
        (address >> 8) & 0xFF,
        address & 0xFF,
		  data & 0xFF
    };

    write_enable();
    GPIOB->BRR = GPIO_PIN_5;
    SPI1_Transmit(cmd, 5);
    GPIOB->BSRR = GPIO_PIN_5;

    wait_until_not_busy();
}

uint8_t read_byte(uint32_t addr) {
    uint8_t cmd[4] = {
        CMD_READ_DATA,
        (addr >> 16) & 0xFF,
        (addr >> 8) & 0xFF,
        addr & 0xFF
    };
    uint8_t data;
    GPIOB->BRR = GPIO_PIN_5;
    SPI1_Transmit(cmd, 4);
    data = SPI1_ReceiveByte();
    GPIOB->BSRR = GPIO_PIN_5;
    return data;
}

void chip_erase(void)
{
	write_enable();
   GPIOB->BRR = GPIO_PIN_5;
   uint8_t cmd[1] = {CHIP_ERASE};
   SPI1_Transmit(cmd, 4);
   GPIOB->BSRR = GPIO_PIN_5;
   wait_until_not_busy();
}

void MEMCONFIG(uint32_t addr)
{

    reset();
    modify_sr3();
	 modify_sr2();
	 modify_sr1();
	 global_unlock();
	 erase_sector(addr);
	 //chip_erase()

}


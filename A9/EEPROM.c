/*
 * EEPROM.c
 *
 *  Created on: May 21, 2025
 *      Author: bdaly
 */
#include "main.h"
#include "EEPROM.h"

//EEPROM INIT
void EEPROM_INIT(void) {
    // Enable I2C1 and GPIO clocks
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
    RCC->APB1ENR1 |= RCC_APB1ENR1_I2C1EN;

    // Configure PB8 (SCL) and PB9 (SDA) for AF4 (I2C1)
    GPIOB->MODER &= ~((3 << (8 * 2)) | (3 << (9 * 2)));   // Clear MODER
    GPIOB->MODER |=  (2 << (8 * 2)) | (2 << (9 * 2));     // Set to AF mode
    GPIOB->OTYPER |= (1 << 8) | (1 << 9);                 // Open-drain
    GPIOB->OSPEEDR |= (3 << (8 * 2)) | (3 << (9 * 2));    // High speed
    GPIOB->AFR[1] |= (4 << ((8 - 8) * 4)) | (4 << ((9 - 8) * 4));  // AF4

    // Reset and configure I2C1
    I2C1->CR1 &= ~I2C_CR1_PE;        // Disable I2C
    I2C1->CR1 &= ~I2C_CR1_ANFOFF;    // Enable analog filter
    I2C1->CR1 &= ~I2C_CR1_DNF;       // Disable digital filter
    I2C1->TIMINGR = TIMINGVAL;      // Set timing from CubeMX
    I2C1->CR1 |= I2C_CR1_PE;         // Enable I2C
}



//EEPROM WRITE

uint8_t EEPROM_WriteByte(uint16_t address, uint16_t data)
{
    I2C1->CR1 &= ~I2C_CR1_PE;
    I2C1->CR1 |= I2C_CR1_PE;

    // Set up 7-bit address write mode for 3 bytes
    I2C1->CR2 = 0;
    I2C1->CR2 |= (EEPROM_ADDR << 1);          // Slave address
    I2C1->CR2 |= (3 << I2C_CR2_NBYTES_Pos);      // 2 addr bytes + 1 data
    I2C1->CR2 &= ~I2C_CR2_RD_WRN;                // Write mode
    I2C1->CR2 |= I2C_CR2_START | I2C_CR2_AUTOEND;

    // Transmit: MSB address
    //check ACK and SEND msb of Address
    	   while (!(I2CBUS->ISR & (I2C_ISR_TXIS | I2C_ISR_NACKF)));
    	   if (I2CBUS->ISR & I2C_ISR_NACKF) {
    	        I2CBUS->ICR |= I2C_ICR_NACKCF;
    	        return ERROR;
    	    }
    	   I2C1->TXDR = address >> 8;

    // Transmit: LSB address
    //check ACK and SEND msb of Address
    	   while (!(I2CBUS->ISR & (I2C_ISR_TXIS | I2C_ISR_NACKF)));
    	   if (I2CBUS->ISR & I2C_ISR_NACKF) {
    	        I2CBUS->ICR |= I2C_ICR_NACKCF;
    	        return ERROR;
    	    }
    	   I2C1->TXDR = address & 0xFF;

    // Transmit: data
    //check ACK and SEND msb of Address
    	   while (!(I2CBUS->ISR & (I2C_ISR_TXIS | I2C_ISR_NACKF)));
    	   if (I2CBUS->ISR & I2C_ISR_NACKF) {
    	        I2CBUS->ICR |= I2C_ICR_NACKCF;
    	        return ERROR;
    	    }
    	   I2C1->TXDR = data;

    // Wait for STOP
    while (!(I2C1->ISR & I2C_ISR_STOPF));
    I2C1->ICR |= I2C_ICR_STOPCF;  // Clear STOP flag
}


//EEPROM READ
uint8_t EEPROM_ReadByte(uint16_t address, uint16_t *data)
{
	    I2C1->CR1 &= ~I2C_CR1_PE;
	    I2C1->CR1 |= I2C_CR1_PE;

	    I2C1->CR2 = 0;
	    I2C1->CR2 |= (EEPROM_ADDR << 1);
	    I2C1->CR2 |= (2 << I2C_CR2_NBYTES_Pos);     // 2 bytes addr
	    I2C1->CR2 &= ~I2C_CR2_RD_WRN;               // Write
	    I2C1->CR2 |= I2C_CR2_START;

	    //check ACK and SEND msb of Address
	    	   while (!(I2CBUS->ISR & (I2C_ISR_TXIS | I2C_ISR_NACKF)));
	    	   if (I2CBUS->ISR & I2C_ISR_NACKF) {
	    	        I2CBUS->ICR |= I2C_ICR_NACKCF;
	    	        return ERROR;
	    	    }
	    	   I2C1->TXDR = address >> 8;
	    	   //check ACK and SEND msb of Address
	    	   	   while (!(I2CBUS->ISR & (I2C_ISR_TXIS | I2C_ISR_NACKF)));
	    	   	   if (I2CBUS->ISR & I2C_ISR_NACKF) {
	    	   	        I2CBUS->ICR |= I2C_ICR_NACKCF;
	    	   	        return ERROR;
	    	   	    }
	    	   	   I2C1->TXDR = address & 0xFF;
	    while (!(I2C1->ISR & I2C_ISR_TC)); // Wait for transfer complete

	    // Second: read 1 byte from set address
	    I2C1->CR2 = 0;
	    I2C1->CR2 |= (EEPROM_ADDR << 1);
	    I2C1->CR2 |= (1 << I2C_CR2_NBYTES_Pos);
	    I2C1->CR2 |= I2C_CR2_RD_WRN | I2C_CR2_START | I2C_CR2_AUTOEND;

	    while (!(I2C1->ISR & I2C_ISR_RXNE));
	    data = I2C1->RXDR;

	    while (!(I2C1->ISR & I2C_ISR_STOPF));
	    I2C1->ICR |= I2C_ICR_STOPCF;

	    return data;
}


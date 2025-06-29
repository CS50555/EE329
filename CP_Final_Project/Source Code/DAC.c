#include "SPI2.h"
#include "SPI.h"
#include "main.h"

/* ---------------------------- (DAC_init) ------------------------------
* Initializes all things needed for the DAC to work
* Inputs: None
* Outputs: None
* Local vars: None
* -------------------------------------------------------------------------- */
void DAC_init(void){
	//!!!!!NEEED TO USE SPIBUS2 for DAC (16 bit mode and NSS -> Look for original code)
	SPI_Config();
	SPI_init();
}

/* ---------------------------- (DAC_volt_conv) ------------------------------
* Takes desired voltage and ouputs it to a value for the dac
* Inputs: float voltage
* Outputs: int value
* Local vars: None
* -------------------------------------------------------------------------- */
int DAC_volt_conv(float voltage ){
	if (voltage > 3.30){
		return (3.30 *4095)/(2* 2.048);;
	}
	if (voltage > 2.04){
		 return (voltage *4095)/(2* 2.048);
	}
	else{
		return (voltage *4095)/(2.048);
	}
}

/* ---------------------------- (DAC_Write) ------------------------------
* Writes the 12 bits to the dac
* Inputs: dac_code --> 12 bits input to dac, gain_bit --> determines gain
* Outputs: None
* Local vars: word --> is the bits output to the SDI
* -------------------------------------------------------------------------- */
void DAC_Write(uint16_t dac_code, uint8_t gain_bit){
	dac_code &= 0xFFF; //Ensures input is only 12 bits
	uint16_t word = (gain_bit << 13) | (1<<12) | dac_code;// Makes 16 bit sequence
	while(SPI1->SR & SPI_SR_BSY);
	SPI1->DR = word;                   //Outputs 12 bits when dac is ready
	while(!(SPI1->SR & SPI_SR_TXE));
	while(SPI1->SR & SPI_SR_BSY);

}



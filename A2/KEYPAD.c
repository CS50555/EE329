#include <KEYPAD.h>

/* ---------------------------- (Keypad_Configuration ) ------------------------------
 * Initializes all the pins as needed
 * Inputs: None
 * Outputs: None
 * Local vars: None
 * -------------------------------------------------------------------------- */
void Keypad_Configuration(void)
{
	  //Initialize clock and inputs and outputs and stuff
	  //Enable GPIOC clock for peripheral
	  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
	  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	  //set GPIO to input mode (Reset bits 6 and 7 for input mode) (00)
	  GPIOC->MODER = 0;


	  //INITIALIZES COLUMNS AND OUTPUTS
	  GPIOB->MODER &= ~(GPIO_MODER_MODE0 | GPIO_MODER_MODE1 | GPIO_MODER_MODE2 | GPIO_MODER_MODE3 | GPIO_MODER_MODE4|GPIO_MODER_MODE5|GPIO_MODER_MODE6 |GPIO_MODER_MODE8);
	  GPIOB->MODER |=  (GPIO_MODER_MODE0_0 | GPIO_MODER_MODE1_0 | GPIO_MODER_MODE2_0 | GPIO_MODER_MODE3_0 |GPIO_MODER_MODE4_0| GPIO_MODER_MODE5_0|GPIO_MODER_MODE6_0| GPIO_MODER_MODE8_0);
	  //Initializes PUPDR
	  GPIOC->PUPDR &= PUPDRST;
	  GPIOC->PUPDR |= (GPIO_PUPDR_PUPD0_1 | GPIO_PUPDR_PUPD1_1 | GPIO_PUPDR_PUPD2_1 | GPIO_PUPDR_PUPD3_1);
	  GPIOB->PUPDR |= (GPIO_PUPDR_PUPD0_0 | GPIO_PUPDR_PUPD1_0 | GPIO_PUPDR_PUPD2_0 | GPIO_PUPDR_PUPD3_0);
	  GPIOC->OTYPER &= (GPIO_OTYPER_OT0 | GPIO_OTYPER_OT1 |GPIO_OTYPER_OT2 |GPIO_OTYPER_OT3);
	  // Set very high output speed for PC0, PC1, PC2, and PC3
	  GPIOB->OSPEEDR |=  (3 << GPIO_OSPEEDR_OSPEED6_Pos);
	  GPIOB->OTYPER &= ~(GPIO_OTYPER_OT6);
	  GPIOB->BSRR = (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2| GPIO_PIN_3); // turn off all LEDs
	  GPIOB->BRR = OUTPUT_PINS;
}

/* ---------------------------- (Key_Pressed ) ------------------------------
 * Detects if a key is pressed with debounce to not bug out
 * Inputs: None
 * Outputs: TRUE (1) or FALSE (0) depending on if a key is pressed
 * Local vars: None
 * -------------------------------------------------------------------------- */
uint8_t Key_Pressed(void) {

	   for ( uint16_t idx=0; idx<SETTLE; idx++ ){// let it settle
	   if ((ROWPORTS->IDR != 0 ))  {      // Detects input (keypress)
	      //for(int j = 0; j < 500; j++);   // Debounce
	      if ((Debounce()) == TRUE)  return( TRUE );
	   }
	   else{
	      return( FALSE );
	   }// nope.
	}
}



/* ---------------------------- (Keypad_WhichKeyIsPressed ) -------------------
 * Detects exactly which key was pressed on the keypad and returns a mapped
 * value
 * Inputs: None
 * Outputs: Mapped binary value corresponds to what key is pressed (int)
 * Local vars: iCol (int) used as counter
 * -------------------------------------------------------------------------- */

int Keypad_WhichKeyIsPressed(void) {
   int iCol;
   COLPORTS->BSRR = COL_PINS;
   if(ROWPORTS->IDR == 0x1){

	   for (iCol = 0; iCol < NUM_COLS; iCol++){  //Detects which column in row 1
	   	   COLPORTS->BRR = COL_PINS;

	   	   		   if (iCol == 0){
	   	   			   COLPORTS->BSRR =  0x1;
	   	   			   if(ROWPORTS->IDR != 0){
	   	   				   	   	   	   COLPORTS->BSRR = COL_PINS;
	   	   			   		   		   return 10;
	   	   			   		   	   }

	   	   		   }
	   	   		   else if (iCol == 1){
	   	   			   COLPORTS->BSRR =  (0x1<<1);
	   	   			   if(ROWPORTS->IDR !=0){
	   	   				   	   	   	   COLPORTS->BSRR = COL_PINS;
	   	   			   		   		   return 3;
	   	   			   		   	   }
	   	   		   }
	   	   		   else if (iCol == 2){
	   	   		   	   COLPORTS->BSRR =(0x1<<2);
	   	   		   	   if(ROWPORTS->IDR !=0){
	   	   		   		   COLPORTS->BSRR = COL_PINS;
	   	   		   		   return 2;
	   	   		   	   }
	   	   		   }
	   	   		   else if (iCol == 3){
	   	   		   COLPORTS->BSRR =  (0x1<<3);
	   	   		   	   if(ROWPORTS->IDR !=0){
	   	   		   	   COLPORTS->BSRR = COL_PINS;
	   	   			   return 1;
	   	   			   		   	   }
	   	   		   }
	   	   	   }
   }
   else if (ROWPORTS->IDR == 0x2){
	   for (iCol = 0; iCol < NUM_COLS; iCol++){ //Detects which column in row 2
	   COLPORTS->BRR = COL_PINS;

	   		   if (iCol == 0){
	   			   COLPORTS->BSRR =  0x1;
	   			   if(ROWPORTS->IDR != 0){
	   				   	   	   	   COLPORTS->BSRR = COL_PINS;
	   			   		   		   return 11;
	   			   		   	   }

	   		   }
	   		   else if (iCol == 1){
	   			   COLPORTS->BSRR =  (0x1<<1);
	   			   if(ROWPORTS->IDR !=0){
	   				   	   	   	   COLPORTS->BSRR = COL_PINS;
	   			   		   		   return 6;
	   			   		   	   }
	   		   }
	   		   else if (iCol == 2){
	   		   	   COLPORTS->BSRR =(0x1<<2);
	   		   	   if(ROWPORTS->IDR !=0){
	   		   		   COLPORTS->BSRR = COL_PINS;
	   		   		   return 5;
	   		   	   }
	   		   }
	   		   else if (iCol == 3){
	   		   COLPORTS->BSRR =  (0x1<<3);
	   		   	   if(ROWPORTS->IDR !=0){
	   		   	   COLPORTS->BSRR = COL_PINS;
	   			   return 4;
	   			   		   	   }
	   		   }
	   		   else{
	   			   continue;
	   		   }
	   	   }
   }
   else if (ROWPORTS->IDR == 0x4){
	   for (iCol = 0; iCol < NUM_COLS; iCol++){ //Detects which column in row 3
	   	   COLPORTS->BRR = COL_PINS;

	   	   		   if (iCol == 0){
	   	   			   COLPORTS->BSRR =  0x1;
	   	   			   if(ROWPORTS->IDR !=0){
	   	   				   	   	   	   COLPORTS->BSRR = COL_PINS;
	   	   			   		   		   return 12;
	   	   			   		   	   }

	   	   		   }
	   	   		   else if (iCol == 1){
	   	   			   COLPORTS->BSRR =  (0x1<<1);
	   	   			   if(ROWPORTS->IDR !=0){
	   	   				   	   	   	   COLPORTS->BSRR = COL_PINS;
	   	   			   		   		   return 9;
	   	   			   		   	   }
	   	   		   }
	   	   		   else if (iCol == 2){
	   	   		   	   COLPORTS->BSRR =(0x1<<2);
	   	   		   	   if(ROWPORTS->IDR !=0){
	   	   		   		   COLPORTS->BSRR = COL_PINS;
	   	   		   		   return 8;
	   	   		   	   }
	   	   		   }
	   	   		   else if (iCol == 3){
	   	   		   COLPORTS->BSRR =  (0x1<<3);
	   	   		   	   if(ROWPORTS->IDR !=0){
	   	   		   	   COLPORTS->BSRR = COL_PINS;
	   	   			   return 7;
	   	   			   		   	   }
	   	   		   }
	   	   		   else{
	   	   			   continue;
	   	   		   }
	   	   	   }
     }
   else if (ROWPORTS->IDR == 0x8){
	   for (iCol = 0; iCol < NUM_COLS; iCol++){  //Detects which column in row 4
	   	   COLPORTS->BRR = COL_PINS;

	   	   		   if (iCol == 0){
	   	   			   COLPORTS->BSRR =  0x1;
	   	   			   if(ROWPORTS->IDR != 0){
	   	   				   	   	   	   COLPORTS->BSRR = COL_PINS;
	   	   			   		   		   return 13;
	   	   			   		   	   }

	   	   		   }
	   	   		   else if (iCol == 1){
	   	   			   COLPORTS->BSRR =  (0x1<<1);
	   	   			   if(ROWPORTS->IDR !=0){
	   	   				   	   	   	   COLPORTS->BSRR = COL_PINS;
	   	   			   		   		   return 14;
	   	   			   		   	   }
	   	   		   }
	   	   		   else if (iCol == 2){
	   	   		   	   COLPORTS->BSRR =(0x1<<2);
	   	   		   	   if(ROWPORTS->IDR !=0){
	   	   		   		   COLPORTS->BSRR = COL_PINS;
	   	   		   		   return 0;
	   	   		   	   }
	   	   		   }
	   	   		   else if (iCol == 3){
	   	   		   COLPORTS->BSRR =  (0x1<<3);
	   	   		   	   if(ROWPORTS->IDR !=0){
	   	   		   	   COLPORTS->BSRR = COL_PINS;
	   	   			   return 15;
	   	   			   		   	   }
	   	   		   }
	   	   		   else{
	   	   			   continue;
	   	   		   }
	   	   	   }
   }



}

/* ---------------------------- (Debounce ) ------------------------------------
 * Debounces keypad pressing by making sure output is constant
 * Inputs: None
 * Outputs: Returns true once input stays contstant
 * Local vars: prevalue to keep track if it was previously one
 * -------------------------------------------------------------------------- */

int Debounce(void){
	int preValue1;
	int preValue2;
	int preValue3;
	while ((preValue1 & preValue2 & preValue3) == 0){
		preValue1 = ROWPORTS->IDR;
		for(int i = 0; i < 200; i++);
		preValue2 = ROWPORTS->IDR;
		for(int i = 0; i < 200; i++);
		preValue3 = ROWPORTS->IDR;
		for(int i = 0; i < 200; i++);
	}
	return TRUE;
}

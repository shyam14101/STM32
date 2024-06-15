#include "stm32f1xx.h"

//Port define function
void GpioInit();

// defining the array
uint16_t digit[] = {
                      0b00111111,
                      0b00000110,
                      0b01011011,
                      0b01001111,
                      0b01100110,
                      0b01101101,
                      0b01111101,
                      0b00000111,
                      0b01111111,
                      0b01101111
                      };

// delay program
void delay(uint32_t a);
int main(){
	SystemInit();

	// configuration of clock for port A
	RCC->APB2ENR = (1<<2);

	// call GpioInit()
	GpioInit();

	while(1){
		for(int i = 0; i< 10 ; i++)
		{
			GPIOA->BSRR =  digit[i] & 0X00FF;
			delay(250000);
			GPIOA->BSRR =  0XFFFF0000;
//			delay(50000);

		}

	}
}


void GpioInit(){

	GPIOA->CRL = (GPIOA->CRL & ~(0XFFFFFFF)) | (0X1111111);
}

void delay(uint32_t a){
	for (int b = 0; b < a ; b++ );
}

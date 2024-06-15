#include "stm32f1xx.h"
// main clock setup predefine function
void SystemInit (void);
int main(){
	SystemInit();

	// initializing clock for port A
	RCC->APB2ENR |= (1<<2);

	// Configure the output pins in port A
	GPIOA->CRL = (GPIOA->CRL & ~(0XFFFFFFFF)) | (0X11111111); //	configure the first 7 pins
	GPIOA->CRH = (GPIOA->CRH & ~(0X00000FFF)) | (0X00000111); // Configure the next pin from 8 to 10

	while(1){

		GPIOA->BSRR = (1<<0);
		for(int i =0; i <=50000; i++);
		GPIOA->BSRR = (1<<1);
		for(int i =0; i<=50000; i++);
		GPIOA->BSRR = (1<<2);
		for(int i =0; i <=50000; i++);
		GPIOA->BSRR = (1<<3);
		for(int i =0; i<=50000; i++);
		GPIOA->BSRR = (1<<4);
		for(int i =0; i <=50000; i++);
		GPIOA->BSRR = (1<<5);
		for(int i =0; i<=50000; i++);
		GPIOA->BSRR = (1<<6);
		for(int i =0; i <=50000; i++);
		GPIOA->BSRR = (1<<7);
		for(int i =0; i<=50000; i++);
		GPIOA->BSRR = (1<<8);
		for(int i =0; i<=50000; i++);
		GPIOA->BSRR = (1<<9);
		for(int i =0; i<=50000; i++);
		GPIOA->BSRR = 1<<(9+16) | 1<<(8+16) | 1<<(7+16) | (1<<(6+16)) | 1<<(5+16) | 1<<(4+16) | 1<<(3+16) | 1<<(2+16) | 1<<(1+16) | 1<<(0+16);
		for(int i =0; i<=50000; i++);

	}
}

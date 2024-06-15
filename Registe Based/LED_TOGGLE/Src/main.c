#include "stm32f1xx.h"

//Gpio initiation
void GpioInit();



int main()
{

	// clock initiation
	RCC->APB2ENR = 0x0004;


	// redeclear GPIO
	GpioInit();
	int lastbtn = 0;
	while(1)
	{
		if(GPIOA->IDR){
			lastbtn = !lastbtn;
			if(lastbtn){
				GPIOA->BSRR = (1<<0);
			}
			else{
				GPIOA->BSRR = 1 << (0+16);
			}
		}
	}
}

void GpioInit(){

	GPIOA->CRL = (GPIOA->CRL & ~(0x0000000F)) | 0x00000001 ;
	GPIOA->CRL = (GPIOA->CRL & ~(0x00000080))  | 0x00000010;
}

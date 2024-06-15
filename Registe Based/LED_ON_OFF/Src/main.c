#include "stm32f1xx.h"






int main(){

	SystemInit();

	//configure clock A for led pin
	RCC->APB2ENR |= (1<<2);
	//CONFIGURE CLOCK B FOR PUSH BUTTON PINS
	RCC->APB2ENR |= (1<<3);

	// CONFIGURE THE PIN A0 FOR OUTPUT
	//clear mode and cnf bits (USING AND OPERATION)
	GPIOA->CRL &= ~(0xF << 0);
	//set mode and cnf bits (USING OR OPERATION)
	GPIOA->CRL |=  (0x2 << 0);

	//CONFIGURATION OF PIN PA1  FOR INPUT
	// clear mode and cnf bits for pin a1
	GPIOA->CRL &= ~(0xF << 4);
	// set mode and cnf bits (using or operation)
	GPIOA->CRL |=  (0x8 << 4);



	// initiation of button state
	int lastbtn = ((GPIOA->IDR & (1<<1)) >> 1);
	int led_state = 0;

	while(1)
	{


		int btn = ((GPIOA->IDR & (1<<1)) >> 1);


		if(btn != lastbtn)
		{

			lastbtn = btn;


			 if (btn == 1) {
				if (led_state == 0) {
					GPIOA->BSRR = (1 << 0);  // Turn on LED
					led_state = 1;
				} else {
					GPIOA->BSRR = (1 << 16); // Turn off LED
					led_state = 0;
				}
			}

		}
	}
}


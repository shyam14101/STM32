#include "stm32f1xx.h"

// initiation of digits
uint16_t digit[]={
		0x3f,
		0x06,
		0x5B,
		0x4F,
		0x66,
		0x6D,
		0x7D,
		0x07,
		0x7F,
		0x6F
};

// delay function to reduce debounce
void Delay_ms(uint32_t ms);

// function to read the btn state
uint8_t read_btn_state();

// count function
void count(uint16_t i);

int main(){
	SystemInit();
	// CONFIGURATION of clock for port A
	RCC->APB2ENR |= (1 << 2);
	//configure port A for seven segment display
	// reset mode and cnf for port A
	GPIOA->CRL &= ~(0xFFFFFFF << 0);
	//set mode and cnf for port A
	GPIOA->CRL |=  (0x2222222 << 0);

	// configure clock for port B for push button
	RCC->APB2ENR |= ( 1 << 3 );
	//reset mode and cnf for pin B0 and B1
	GPIOB->CRL &= ~(0xF << 8) | (0xF << 4);
	//set mode and cnf for pin B0 and B1
	GPIOB->CRL |=  (0x8 << 8) | (0x8 << 4);
	//set the internal pull up resistor
//	GPIOB->CRL |= (0x11 << 0);

	//initializing count value
	uint16_t i = 0;

	while(1){
		//read btn state with other variable
		uint8_t btnstate = read_btn_state();


		        if (btnstate & 0x01) {
		            // Button 2 (PA2) is pressed
		        	i--;
//		        	Delay_ms(50);
		        	count(i);

		        }

		        if (btnstate & 0x02) {
		            // Button 1 (PA1) is pressed
		        	i++;
//		        	Delay_ms(50);
		        	count(i);


		        } else {
		        	count(i);
		            // Button 1 (PA1) is not pressed
		        }

	}
}

void Delay_ms(uint32_t ms){
	for(uint32_t i = 0; i < ms *8000; i++){

	}
}

uint8_t read_btn_state(){
	uint8_t btn_state1 = (GPIOB->IDR & (1<<1)) >> 1;
	uint8_t btn_state2 = (GPIOB->IDR & (1<<2)) >> 2;

	return ((btn_state1 << 1) | btn_state2);
}

void count(uint16_t i){
	GPIOA->BSRR =  digit[i] << 0;
    Delay_ms(8);
    GPIOA->BSRR =  digit[i] << 16;
}


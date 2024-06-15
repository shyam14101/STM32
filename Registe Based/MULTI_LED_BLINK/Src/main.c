#include "stm32f1xx.h"

void clockInit(uint8_t a);
void pinInit();
int main(void){
    // clock initializing
    clockInit(2);
    clockInit(3);
    // conf pin 1 on gpioa
    GPIOA->CRL |= ( (1<<4) | (0<<5) | (1<<8) | (0<<9) );
    GPIOA->CRL &= ~( (1<<6) | (1<<7) | (1<<10) | (1<<11) );
    GPIOB->CRL |= ( (1<<4) | (0<<5) | (1<<8) | (0<<9) );
    GPIOB->CRL &= ~( (1<<6) | (1<<7) | (1<<10) | (1<<11) );

    while (1)
    {
        GPIOA->BSRR = ( (1<<1) | (1<<2) );//set the pin to high
        GPIOB->BSRR = ( (1<<1) | (1<<2) );//set the pin to high
        for (int i = 0; i <=500000; i++);
        GPIOA->BSRR = ( 1<<(1 + 16) | 1<<(2 + 16) ); //reset the pin to low
        GPIOB->BSRR = ( 1<<(1 + 16) | 1<<(2 + 16) ); //reset the pin to low
        for (int i = 0; i <=500000; i++);
    }

}

void clockInit(uint8_t a){
	RCC->APB2ENR |= (1<<a);
}

void pinInit(){

}

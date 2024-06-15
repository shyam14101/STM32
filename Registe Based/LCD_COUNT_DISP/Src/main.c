#include "stm32f1xx.h"


// define lcd ports
#define lcd_RS 8
#define lcd_EN 9
#define lcd_port GPIOA
#define btn_port GPIOB
#define btn_B0 0
#define btn_B1 1
uint16_t data[8] = {0,1,2,3,4,5,6,7};

//lcd initiate
void lcdInit();

// print function
void lcdPrint(uint8_t input);

// print cmd function
void lcdCommand(uint8_t cmd);
// lcd enable
void lcdEnable(void);

// Delay function
void Delay(uint32_t ms);

//btn init
void btnInit();

// read btn state
uint8_t read_btnstate();



int main(void){
	SystemInit();
	lcdInit();
	btnInit();

	lcdCommand(0x30);
	lcdCommand(0x30);
	lcdCommand(0x30);
	lcdCommand(0x0C);
	lcdCommand(0x0F);
	lcdCommand(0x0E);
	lcdCommand(0x01);
	lcdCommand(0x10);


	uint16_t i = 0;


	while(1){
		lcdCommand(0x80);


		uint8_t btnstate = read_btnstate();


			if (btnstate & 0x02) {
				// Button 2 (PA1) is pressed
				i--;
				lcdCommand(0x10);
				lcdPrint(i + 48);

			}

			if (btnstate & 0x01) {
				// Button 1 (PA0) is pressed
				i++;

				lcdPrint(i + 48);


			} else {

				lcdPrint(i + 48);
				// Button 1 (PA0) and (PA1) is not pressed
			}


	}
}
//complete lcd functions
void lcdInit(){

	// clock init
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN ;

	// rs configuration
	if(lcd_RS < 8){
		lcd_port->CRL &= ~(0xF << (4 * lcd_RS) );
		lcd_port->CRL |=  (0x2 << (4 * lcd_RS ));
	}
	else{
		lcd_port->CRH &= ~(0xF << (4 * ( lcd_RS - 8)));
		lcd_port->CRH |=  (0x2 << (4 * ( lcd_RS - 8)));
	}
	//en configuration
	if(lcd_EN < 8){
		lcd_port->CRL &= ~(0xF << (4 * lcd_EN) );
		lcd_port->CRL |=  (0x2 << (4 * lcd_EN) );
	}
	else{
		lcd_port->CRH &= ~(0xF << (4 * ( lcd_EN - 8)));
		lcd_port->CRH |=  (0x2 << (4 * ( lcd_EN - 8)));
	}
	// configure pins
	for(int i = 0; i < 8; i++){
		if(data[i] < 8){
			lcd_port->CRL &= ~(0xF << (4 * data[i]));
			lcd_port->CRL |=  (0x2 << (4 * data[i]));

		}
		else{
			lcd_port->CRH &= ~(0xF << (4 * (data[i] - 8)));
			lcd_port->CRH |=  (0x2 << (4 * (data[i] - 8)));
		}
	}
}

void lcdPrint(uint8_t input){
	//rs ==1
	lcd_port->BSRR = (1 << lcd_RS);
	//set data pins
	for (int i = 0; i < 8; i++) {
		if (input & (1 << i)) {
			lcd_port->BSRR = (1 << data[i]);
		} else {
			lcd_port->BSRR = (1 << (data[i] + 16));
		}
	}
	//call lcd enable
	lcdEnable();
	Delay(6);
}

void lcdCommand(uint8_t cmd){
	// rs = 0
	lcd_port->BSRR = (1 << (lcd_RS +16));
	//set data pins
	 for (int i = 0; i < 8; i++) {
		if (cmd & (1 << i)) {
			lcd_port->BSRR = (1 << data[i]);
		} else {
			lcd_port->BSRR = (1 << (data[i] + 16));
		}
	}
	lcdEnable();

}

void lcdEnable(void){

	lcd_port->BSRR = (1 << lcd_EN);
	Delay(1);
	lcd_port->BSRR = (1 << (lcd_EN +16));
	Delay(1);
}

//complete btn function
void btnInit(){

	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

	//PORTB0
	if(btn_B0 < 8){
		btn_port->CRL &= ~(0xF << (4 * btn_B0) );
		btn_port->CRL |=  (0x8 << (4 * btn_B0 ));
	}
	else{
		btn_port->CRH &= ~(0xF << (4 * ( btn_B0 - 8)));
		btn_port->CRH |=  (0x8 << (4 * ( btn_B0 - 8)));
	}
	//PORTB1 configuration
	if(btn_B1 < 8){
		btn_port->CRL &= ~(0xF << (4 * btn_B1) );
		btn_port->CRL |=  (0x8 << (4 * btn_B1) );
	}
	else{
		btn_port->CRH &= ~(0xF << (4 * ( btn_B1 - 8)));
		btn_port->CRH |=  (0x8 << (4 * ( btn_B1 - 8)));
	}


}

uint8_t read_btnstate(){
	uint8_t btn1_state = (btn_port->IDR & (1 << 2)) >> 2;
	uint8_t btn2_state = (btn_port->IDR & (1 << 1)) >> 1;

	return ((btn1_state << 1) | btn2_state ) ;
}
//common code
void Delay(uint32_t ms){
	for(int i = 0; i< ms*8000; i++);
}

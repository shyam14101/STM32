#include "stm32f1xx.h"

// Define structure to hold LCD configuration
typedef struct {
    GPIO_TypeDef *PortRS;
    uint16_t PinRS;
    GPIO_TypeDef *PortEN;
    uint16_t PinEN;
    GPIO_TypeDef *PortData;
    uint16_t DataPins[8];
} LCD_Config;

// Declare an instance of LCD_Config
LCD_Config lcdConfig;

// Function prototypes
void lcdSetup(GPIO_TypeDef *PortRS, uint16_t PinRS, GPIO_TypeDef *PortEN, uint16_t PinEN, GPIO_TypeDef *PortData,
              uint16_t D0, uint16_t D1, uint16_t D2, uint16_t D3, uint16_t D4, uint16_t D5, uint16_t D6, uint16_t D7);
void LCD_Init(void);
void LCD_SendCommand(uint8_t cmd);
void LCD_SendData(uint8_t data);
void LCD_Enable(void);
void Delay_ms(uint32_t ms);

int main(void) {
    SystemInit();

    // Setup LCD with the specified pins
    lcdSetup(GPIOA, 8, GPIOA, 9, GPIOA, 0, 1, 2, 3, 4, 5, 6, 7);

    // Initialize the LCD
    LCD_Init();

    while (1) {
        LCD_SendCommand(0x01);  // Clear display
        Delay_ms(2);

        LCD_SendCommand(0x80);  // Move cursor to the beginning of the first line
        LCD_SendData('S');
        LCD_SendData('H');
        LCD_SendData('Y');
        LCD_SendData('A');
        LCD_SendData('M');

        LCD_SendCommand(0xC0);  // Move cursor to the beginning of the second line
        LCD_SendData('S');
        LCD_SendData('U');
        LCD_SendData('N');
        LCD_SendData('D');
        LCD_SendData('E');
        LCD_SendData('R');
        LCD_SendData('.');
        LCD_SendData('R');

        while (1);  // Infinite loop
    }
}

void lcdSetup(GPIO_TypeDef *PortRS, uint16_t PinRS, GPIO_TypeDef *PortEN, uint16_t PinEN, GPIO_TypeDef *PortData,
              uint16_t D0, uint16_t D1, uint16_t D2, uint16_t D3, uint16_t D4, uint16_t D5, uint16_t D6, uint16_t D7) {
    lcdConfig.PortRS = PortRS;
    lcdConfig.PinRS = PinRS;
    lcdConfig.PortEN = PortEN;
    lcdConfig.PinEN = PinEN;
    lcdConfig.PortData = PortData;
    lcdConfig.DataPins[0] = D0;
    lcdConfig.DataPins[1] = D1;
    lcdConfig.DataPins[2] = D2;
    lcdConfig.DataPins[3] = D3;
    lcdConfig.DataPins[4] = D4;
    lcdConfig.DataPins[5] = D5;
    lcdConfig.DataPins[6] = D6;
    lcdConfig.DataPins[7] = D7;
}

void LCD_Init(void) {
    // Enable clock for the specified GPIO ports
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN;

    // Configure RS pin as output
    if (lcdConfig.PinRS < 8) {
        lcdConfig.PortRS->CRL &= ~(0xF << (4 * lcdConfig.PinRS));
        lcdConfig.PortRS->CRL |= (0x2 << (4 * lcdConfig.PinRS));
    } else {
        lcdConfig.PortRS->CRH &= ~(0xF << (4 * (lcdConfig.PinRS - 8)));
        lcdConfig.PortRS->CRH |= (0x2 << (4 * (lcdConfig.PinRS - 8)));
    }

    // Configure EN pin as output
    if (lcdConfig.PinEN < 8) {
        lcdConfig.PortEN->CRL &= ~(0xF << (4 * lcdConfig.PinEN));
        lcdConfig.PortEN->CRL |= (0x2 << (4 * lcdConfig.PinEN));
    } else {
        lcdConfig.PortEN->CRH &= ~(0xF << (4 * (lcdConfig.PinEN - 8)));
        lcdConfig.PortEN->CRH |= (0x2 << (4 * (lcdConfig.PinEN - 8)));
    }

    // Configure Data pins as output
    for (int i = 0; i < 8; i++) {
        if (lcdConfig.DataPins[i] < 8) {
            lcdConfig.PortData->CRL &= ~(0xF << (4 * lcdConfig.DataPins[i]));
            lcdConfig.PortData->CRL |= (0x2 << (4 * lcdConfig.DataPins[i]));
        } else {
            lcdConfig.PortData->CRH &= ~(0xF << (4 * (lcdConfig.DataPins[i] - 8)));
            lcdConfig.PortData->CRH |= (0x2 << (4 * (lcdConfig.DataPins[i] - 8)));
        }
    }

    // Initialization sequence
    Delay_ms(40);  // Wait for more than 30ms after power on
    LCD_SendCommand(0x30);  // Function set
    Delay_ms(5);   // Wait for more than 4.1ms
    LCD_SendCommand(0x30);  // Function set
    Delay_ms(1);   // Wait for more than 100us
    LCD_SendCommand(0x30);  // Function set

    // Function set: 8-bit mode, 2 lines, 5x8 dots
    LCD_SendCommand(0x38);

    // Display control: display on, cursor off, blink off
    LCD_SendCommand(0x0C);

    // Clear display
    LCD_SendCommand(0x01);
    Delay_ms(2);

    // Entry mode set: increment cursor, no shift
    LCD_SendCommand(0x06);
}

void LCD_SendCommand(uint8_t cmd) {
    // RS = 0 for command
    lcdConfig.PortRS->BSRR = (1 << (lcdConfig.PinRS + 16));

    // Set the data pins
    for (int i = 0; i < 8; i++) {
        if (cmd & (1 << i)) {
            lcdConfig.PortData->BSRR = (1 << lcdConfig.DataPins[i]);
        } else {
            lcdConfig.PortData->BSRR = (1 << (lcdConfig.DataPins[i] + 16));
        }
    }

    // Pulse the enable pin
    LCD_Enable();
}

void LCD_SendData(uint8_t data) {
    // RS = 1 for data
    lcdConfig.PortRS->BSRR = (1 << lcdConfig.PinRS);

    // Set the data pins
    for (int i = 0; i < 8; i++) {
        if (data & (1 << i)) {
            lcdConfig.PortData->BSRR = (1 << lcdConfig.DataPins[i]);
        } else {
            lcdConfig.PortData->BSRR = (1 << (lcdConfig.DataPins[i] + 16));
        }
    }

    // Pulse the enable pin
    LCD_Enable();
}

void LCD_Enable(void) {
    // Enable high
    lcdConfig.PortEN->BSRR = (1 << lcdConfig.PinEN);
    Delay_ms(1);  // Wait for the enable pulse width

    // Enable low
    lcdConfig.PortEN->BSRR = (1 << (lcdConfig.PinEN + 16));
    Delay_ms(1);  // Wait for the enable cycle time
}

void Delay_ms(uint32_t ms) {
    // Simple delay loop
    for (uint32_t i = 0; i < ms * 8000; i++);
}

#include "CAN.h"
#include "stm32f10x.h"                  // Device header

#include <stdio.h>

volatile uint32_t idDataRcv;
volatile uint32_t dlcTimestampRcv;
volatile uint32_t lowdataRcv;
volatile uint32_t highdataRcv;
volatile uint8_t flagOvr;
volatile uint8_t flagMailbox0 = 1;

void GPIOconfig(void) {
    // Enable clock for GPIOA, GPIOC, AFIO
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;

		  // --------- PA11 (CAN_RX) ---------
    // Input Floating: CNF11 = 01, MODE11 = 00 ? 0b0100 = 0x4
    GPIOA->CRH &= ~(0xF << 12);     // Clear PA11 config
    GPIOA->CRH |=  (0x4 << 12);

    // --------- PA12 (CAN_TX) ---------
    // AF Push-Pull, 50 MHz: CNF12 = 10, MODE12 = 11 ? 0b1011 = 0xB
    GPIOA->CRH &= ~(0xF << 16);     // Clear PA12 config
    GPIOA->CRH |=  (0xB << 16);
	
		// --------- CAN Pin Remap (NO REMAP � use PA11 & PA12) ---------
    AFIO->MAPR &= ~(0x3 << 13);     // 00: default mapping
	
    // PC13: Output push-pull 2 MHz
    GPIOC->CRH &= ~(GPIO_CRH_MODE13 | GPIO_CRH_CNF13);
    GPIOC->CRH |= (0x2 << 20); // MODE13 = 10b (2MHz), CNF13 = 00 (push-pull)
		GPIOC->BSRR = (1 << 13);  //LED OFF initially

//   ////PA4, PA5: output push pull (SCL, SDA)
//  GPIOA->CRL &= ~(0xFFFFFF);
//  GPIOA->CRL |=  0x332222;
}

int main(void) {
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    RCC->APB1ENR |= RCC_APB1ENR_CAN1EN;
    GPIOconfig();      
    LCD_Init();
    LCD_Clear();
    LCD_test();

    interruptRcvF0Config();
    interruptTransmitConfig();
    NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);
    NVIC_EnableIRQ(USB_HP_CAN1_TX_IRQn);
	
    CAN_InitializationModeSetting();
    FilterBankConfig();
   // CAN_Test_LoopbackModeSetting();
    CAN_NormalModeSetting();
	
    uint8_t msg[8] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};
    while (1) {
			
        if (flagMailbox0 == 1) {
            flagMailbox0 = 0;
            CAN_SendFrame(0x2, msg, 8);
        }
        
        uint8_t ide = (uint8_t)((idDataRcv >> 2) & 0x1);
        uint16_t stid = (uint16_t)((idDataRcv >> 21) & 0x7FF);
        
        if (!ide && stid == 0x2) {
            GPIOC->BRR = (1 << 13);   //LED ON
        } else {
            GPIOC->BSRR = (1 << 13);  //LED OFF
        }
        
        if (flagOvr) {
            GPIOC->BRR = (1 << 13);
            flagOvr = 0;
        }
    }
}


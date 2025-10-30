#include "CAN.h"

volatile uint32_t idDataRcv;
volatile uint32_t dlcTimestampRcv;
volatile uint32_t lowdataRcv;
volatile uint32_t highdataRcv;

void GPIOconfig(void){
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;     // Enable clock for GPIOC

    // PC13: Output push-pull 2 MHz
    GPIOC->CRH &= ~(GPIO_CRH_MODE13 | GPIO_CRH_CNF13);
    GPIOC->CRH |=  (0x2 << 20); // MODE13 = 10b (2MHz), CNF13 = 00 (push-pull)
}

int main(void){
    // Clock CAN + GPIO
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    RCC->APB1ENR |= RCC_APB1ENR_CAN1EN;
		
    GPIOconfig();
		 GPIOC->BSRR = (1 << 13);  // LED OFF
	
    CAN_Test_LoopbackModeSetting();

    while (1) {
        TransmitMailboxConfig();     // send STID=0x2
        RecieveMailboxConfig();      // receive msg through global variable & release FIFO

        
        uint8_t ide = (uint8_t)((idDataRcv >> 2) & 0x1);
        uint16_t stid = (uint16_t)((idDataRcv >> 21) & 0x7FF);

        if (!ide && stid == 0x2) {
						 GPIOC->BRR = (1 << 13);	// LED ON
						
        } else {
             GPIOC->BSRR = (1 << 13);  // LED OFF
        }
				
    }
}

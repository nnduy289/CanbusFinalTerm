#include "CAN.h"

volatile uint32_t idDataRcv;
volatile uint32_t dlcTimestampRcv;
volatile uint32_t lowdataRcv;
volatile uint32_t highdataRcv;
volatile uint8_t flagOvr;

volatile uint8_t flagMailbox0 = 1;

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
		
		interruptRcvF0Config();
		interruptTransmitConfig();
		//Enable interrupt for CAN1_Receive FIFO0
		NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);
		NVIC_EnableIRQ(USB_HP_CAN1_TX_IRQn);
    GPIOconfig();
		GPIOC->BSRR = (1 << 13);  // LED OFF

	CAN_InitializationModeSetting();    //initialize CAN bus
    FilterBankConfig(); //dual 16 bit, identifier list mode, filter bank number 0,... 
    CAN_Test_LoopbackModeSetting();

    while (1) {
       // TransmitMailboxConfig();     // send STID=0x2
       // RecieveMailboxConfig();      // receive msg through global variable & release FIFO

        if(flagMailbox0 == 1){
					flagMailbox0 = 0;	//RESET flag
						//RTR[1]: 0\IDE[2]: 0\EXID[3:20]: 0\STID: 0x1 or 0x2
					CAN1->sTxMailBox[0].TIR = 0x00400000; //STID = 0x2
					CAN1->sTxMailBox[0].TDTR = 0;
					CAN1->sTxMailBox[0].TDTR |= 1 << 3; //8 bytes data, 2 byte cuoi luu TIMER CAPTURE value
				//	CAN1->sTxMailBox[0].TDTR |= 1 << 8; //transmit global time => tu dong truyen 16-bit timer value captured at the SOF transmission
					
					//after complete config transmit mailbox, start request to transmit
					CAN1->sTxMailBox[0].TIR |= CAN_TI0R_TXRQ;
				}
				
        uint8_t ide = (uint8_t)((idDataRcv >> 2) & 0x1);
        uint16_t stid = (uint16_t)((idDataRcv >> 21) & 0x7FF);

        if (!ide && stid == 0x2) {  //check if id is 0x2
						 GPIOC->BRR = (1 << 13);	// LED ON
						
        } else {
             GPIOC->BSRR = (1 << 13);  // LED OFF
        } 
			// if(flagOvr){
			// 	GPIOC->BRR = (1 << 13);	// LED ON
			// 	flagOvr = 0;
			// }
				
    }
}

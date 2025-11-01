#include "CAN.h"

// Global variable in main.c
extern volatile uint32_t idDataRcv;
extern volatile uint32_t dlcTimestampRcv;
extern volatile uint32_t lowdataRcv;
extern volatile uint32_t highdataRcv;
extern volatile uint8_t flagOvr;
//extern volatile uint8_t flagFullFiFo;

void RecieveMailboxConfig(){
    // wait for pending msg come => FIFO had msg
    while ((CAN1->RF0R & 0x3) == 0) { /* wait */ }

    idDataRcv        = CAN1->sFIFOMailBox[0].RIR;
    dlcTimestampRcv  = CAN1->sFIFOMailBox[0].RDTR;
    lowdataRcv       = CAN1->sFIFOMailBox[0].RDLR;
    highdataRcv      = CAN1->sFIFOMailBox[0].RDHR;

    // RELEASE FIFO0
    CAN1->RF0R |= CAN_RF0R_RFOM0;
}

void interruptRcvF0Config(){
    CAN1->IER |= CAN_IER_FMPIE0; //FIFO msg pending interrupt enable,  Interrupt generated when state of FMP[1:0] bits are not 00b.
		CAN1->IER |= CAN_IER_FFIE0;	//FIFO full interrupt enable
		CAN1->IER |= CAN_IER_FOVIE0;//FIFO overrun (over 3 pending msg and continue recv new msg)
	//	CAN1->IER |= CAN_IER_WKUIE; //wakeup interrupt, generated when WKUI bit is set (MSR reg)
	//	CAN1->IER |= CAN_IER_SLKIE; //sleep interrupt, generated when SLAKI bit is set (MSR reg)
		//error, bus-off handler ...
}

void USB_LP_CAN1_RX0_IRQHandler(){
		if(CAN1->RF0R & CAN_RF0R_FMP0){
			idDataRcv        = CAN1->sFIFOMailBox[0].RIR;
			dlcTimestampRcv  = CAN1->sFIFOMailBox[0].RDTR;
			lowdataRcv       = CAN1->sFIFOMailBox[0].RDLR;
			highdataRcv      = CAN1->sFIFOMailBox[0].RDHR;

			//RELEASE FIFO0
			CAN1->RF0R |= CAN_RF0R_RFOM0;
		}
		if(CAN1->RF0R & CAN_RF0R_FULL0){ 	//FIFO is full
			//CAN1->RF0R |= CAN_RF0R_RFOM0; //release msg to store next msg
			CAN1->RF0R |= CAN_RF0R_FULL0;
			//turn on LED to indicate 
		}
		if(CAN1->RF0R & CAN_RF0R_FOVR0){	//FIFO is overload (full and new msg pass filter, come to FIFO)
			//CAN1->RF0R |= CAN_RF0R_RFOM0; //release msg to store next msg
			//turn on LED to indicate
			CAN1->RF0R |= CAN_RF0R_FOVR0;
			flagOvr = 1;
		}
}

void interruptRcvF1Config(){
    CAN1->IER |= CAN_IER_FMPIE1; //FIFO msg pending interrupt enable,  Interrupt generated when state of FMP[1:0] bits are not 00b.
		CAN1->IER |= CAN_IER_FFIE1;	//FIFO full interrupt enable
		CAN1->IER |= CAN_IER_FOVIE1;//FIFO overrun (over 3 pending msg and continue recv new msg)
	//	CAN1->IER |= CAN_IER_WKUIE; //wakeup interrupt, generated when WKUI bit is set (MSR reg)
	//	CAN1->IER |= CAN_IER_SLKIE; //sleep interrupt, generated when SLAKI bit is set (MSR reg)
		//error handler ...
}


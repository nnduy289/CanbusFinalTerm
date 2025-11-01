#include "CAN.h"

void CAN_InitializationModeSetting(){
		CAN1->MCR &= ~CAN_MCR_SLEEP; //clear sleep bit to wakeup 
		while(CAN1->MSR & CAN_MSR_SLAK);
		CAN1->MCR |= CAN_MCR_INRQ;	//enter initialization mode
		while(!(CAN1->MSR & CAN_MSR_INAK)); 	//wait for hardware confirmed request to enter initialization mode
		//set Baudrate 9600 bit/s (16tq) => BRP = 233, TS1 = 12 => BS1 = 13tq, TS2 = 1 => BS2 = 2tq
		CAN1->BTR = 0;	//reset register 
		CAN1->BTR |= (12 << 16);	//TS1 = 12
		CAN1->BTR |= (1 << 20);	//TS2 = 1
		CAN1->BTR |= (233 & 0x3FF); 
		
		//set option for CAN bus
		CAN1->MCR &= ~CAN_MCR_TXFP;	//priority driven by the identifier of the msg: low id => high priority
		CAN1->MCR |= CAN_MCR_RFLM;	//msg will overwrite the previous one once FIFO full
		CAN1->MCR &= ~CAN_MCR_NART; //automatically retransmit msg until it has been successfully transmitted according to the CAN standard
		CAN1->MCR |= CAN_MCR_AWUM; //hardware tu danh thuc khi phat hien tin hieu CAN hop le
		CAN1->MCR |= CAN_MCR_ABOM; //hardware handle automatic bus-off management recovery
	//	CAN1->MCR |= CAN_MCR_TTCM; //time triggered communication
		CAN1->MCR &= ~(1 << 16); //CAN working during debug
	//filter banks(mode, scale, FIFO assignment, activation, filter values).....
		
}

void CAN_NormalModeSetting(){
	//enter normal mode by clear SLEEP bit and INRQ bit in CAN_MCR reg
		CAN1->MCR &= ~CAN_MCR_SLEEP; //clear sleep bit
		while(CAN1->MSR & CAN_MSR_SLAK); //wait for hardware
		CAN1->MCR &= ~CAN_MCR_INRQ;	// clear INRQ bit
		while(CAN1->MSR & CAN_MSR_INAK); //wait for hardware
	//waiting for the occurrence of a sequence of 11 consecutive recessive bits (Bus Idle state) ?
}

void CAN_SleepModeSetting(){
		CAN1->MCR &= ~CAN_MCR_INRQ;	//exit initialization mode
		while(CAN1->MCR & CAN_MSR_INAK); //wait for hardware 
		//enter sleep mode to save energy
		CAN1->MCR |= CAN_MCR_SLEEP; //set sleep bit to enter low-energy mode
		while(!(CAN1->MSR & CAN_MSR_SLAK)); //wait for hardware
}

void CAN_Test_SilentModeSetting(){
		//CAN_InitializationModeSetting(); //enter initialization mode
		CAN1->BTR |= CAN_BTR_SILM; //silent mode: analyze traffic
	
		CAN1->MCR &= ~CAN_MCR_INRQ;	//exit init mode
    while (CAN1->MSR & CAN_MSR_INAK);
}

void CAN_Test_LoopbackModeSetting(){
   // CAN_InitializationModeSetting();   // v�o init
    CAN1->BTR |= CAN_BTR_LBKM;         // loopback

		//exit init mode
    CAN1->MCR &= ~CAN_MCR_INRQ;
    while (CAN1->MSR & CAN_MSR_INAK);
}


void CAN_Test_HotSelfTest(){
		//CAN_InitializationModeSetting();	//enter initialization mode
	//combine silent+loopback
		CAN1->BTR |= CAN_BTR_SILM; //silent mode: analyze traffic
		CAN1->BTR |= CAN_BTR_LBKM; //loopback mode: selftest
	
		CAN1->MCR &= ~CAN_MCR_INRQ;		//exit init mode
    while (CAN1->MSR & CAN_MSR_INAK);
}
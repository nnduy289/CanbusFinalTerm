#include "CAN.h"

//test maibox[0]
void TransmitMailboxConfig(){
	//RTR[1]: 0\IDE[2]: 0\EXID[3:20]: 0\STID: 0x1 or 0x2
	CAN1->sTxMailBox[0].TIR = 0x00200000; //STID = 0x1
	CAN1->sTxMailBox[0].TDTR = 0;
	CAN1->sTxMailBox[0].TDTR |= 1 << 3; //8 bytes data, 2 byte cuoi luu TIMER CAPTURE value
	CAN1->sTxMailBox[0].TDTR |= 1 << 8; //transmit global time => tu dong truyen 16-bit timer value captured at the SOF transmission
	
	//after complete config transmit mailbox, start request to transmit
	CAN1->sTxMailBox[0].TIR |= CAN_TI0R_TXRQ;
	while ((CAN1->TSR & CAN_TSR_RQCP0) == 0); //wait for complete transfer
	CAN1->TSR |= CAN_TSR_RQCP0;               // clear flag
}



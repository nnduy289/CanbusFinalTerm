#include "CAN.h"

extern volatile uint8_t flagMailbox0;
//test maibox[0]
void TransmitMailboxConfig(){
	//RTR[1]: 0\IDE[2]: 0\EXID[3:20]: 0\STID: 0x1 or 0x2
	CAN1->sTxMailBox[0].TIR = 0x00400000; //STID = 0x2
	CAN1->sTxMailBox[0].TDTR = 0;
	CAN1->sTxMailBox[0].TDTR |= 1 << 3; //8 bytes data, 2 byte cuoi luu TIMER CAPTURE value
//	CAN1->sTxMailBox[0].TDTR |= 1 << 8; //transmit global time => tu dong truyen 16-bit timer value captured at the SOF transmission
	
	//after complete config transmit mailbox, start request to transmit
	CAN1->sTxMailBox[0].TIR |= CAN_TI0R_TXRQ;
	while ((CAN1->TSR & CAN_TSR_RQCP0) == 0); //wait for complete transfer
	CAN1->TSR |= CAN_TSR_RQCP0;               // clear flag
}


void interruptTransmitConfig(){
	CAN1->IER |= CAN_IER_TMEIE; //transmit mailbox empty interrupt enable
}

void USB_HP_CAN1_TX_IRQHandler(void) {
    uint32_t tsr = CAN1->TSR;

    if (tsr & CAN_TSR_RQCP0) {
        CAN1->TSR |= CAN_TSR_RQCP0;
        flagMailbox0 = 1; 
    }
    if (tsr & CAN_TSR_RQCP1) {
        CAN1->TSR |= CAN_TSR_RQCP1;//clear flag
        //handle mailbox1
    }
    if (tsr & CAN_TSR_RQCP2) {
        CAN1->TSR |= CAN_TSR_RQCP2;//clear flag
        //handle mailbox2
    }
}
CAN_TxStatus CAN_SendFrame(uint16_t stdId, uint8_t *data, uint8_t len) {
    if (len > 8) len = 8;
    uint8_t mailbox;
    
    if (CAN1->TSR & CAN_TSR_TME0) mailbox = 0;
    else if (CAN1->TSR & CAN_TSR_TME1) mailbox = 1;
    else if (CAN1->TSR & CAN_TSR_TME2) mailbox = 2;
    else return CAN_TX_BUSY;
    
    //Setup data
    uint32_t low = 0, high = 0;
    for (int i = 0; i < 4 && i < len; i++) 
        low |= ((uint32_t)data[i]) << (8 * i);
    for (int i = 4; i < len; i++) 
        high |= ((uint32_t)data[i]) << (8 * (i - 4));
    
    CAN1->sTxMailBox[mailbox].TIR = (stdId << 21);  
    CAN1->sTxMailBox[mailbox].TDTR = 0;
    CAN1->sTxMailBox[mailbox].TDTR |= len;       
    CAN1->sTxMailBox[mailbox].TDLR = low;         
    CAN1->sTxMailBox[mailbox].TDHR = high;      
    CAN1->sTxMailBox[mailbox].TIR |= CAN_TI0R_TXRQ; 
    
    return CAN_TX_OK;
}

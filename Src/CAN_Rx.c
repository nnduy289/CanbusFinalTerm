#include "CAN.h"

void RecieveMailboxConfig(volatile uint32_t idDataRcv, volatile uint32_t dlcTimestampRcv, volatile uint32_t lowdataRcv,volatile uint32_t highdataRcv){
		//receive as FIFO0
		while((CAN1->RF0R & 0x3) != 0); //wait for FMP0 at least 1 pending msg
		idDataRcv = CAN1->sFIFOMailBox[0].RIR;
		dlcTimestampRcv = CAN1->sFIFOMailBox[0].RDTR;
		lowdataRcv = CAN1->sFIFOMailBox[0].RDLR;
		highdataRcv = CAN1->sFIFOMailBox[0].RDHR;
		//release output mailbox after complete read
		CAN1->RF0R |= (1 << 5);
		
}
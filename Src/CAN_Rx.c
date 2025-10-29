#include "CAN.h"

void RecieveMailboxConfig(){
		//receive as FIFO0
		while((CAN1->RF0R & 0x3) != 0); //wait for FMP0 at least 1 pending msg
		uint32_t idDataRcv = CAN1->sFIFOMailBox[0].RIR;
		uint32_t dlcTimestampRcv = CAN1->sFIFOMailBox[0].RDTR;
		uint32_t lowdataRcv = CAN1->sFIFOMailBox[0].RDLR;
		uint32_t highdataRcv = CAN1->sFIFOMailBox[0].RDHR;
		//release output mailbox after complete read
		CAN1->RF0R |= (1 << 5);
		
		//check 11 bit identifier id 
		if((idDataRcv & 0x8FF) == 0x1){
			//turn on LED PA0
		}else{
			//turn off led PA0
		}
		
}
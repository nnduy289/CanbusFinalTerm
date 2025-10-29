#include "CAN.h"

// Dùng luôn bi?n global dã khai báo trong main.c
extern volatile uint32_t idDataRcv;
extern volatile uint32_t dlcTimestampRcv;
extern volatile uint32_t lowdataRcv;
extern volatile uint32_t highdataRcv;

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

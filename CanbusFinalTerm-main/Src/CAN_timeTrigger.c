#include "CAN.h"

void CAN_TimeTriggerEnable(void){
    CAN1->MCR |= CAN_MCR_TTCM; //enable TTCM mode
}

void CAN_TimeTriggerDisable(void){
    CAN1->MCR &= ~CAN_MCR_TTCM;
}
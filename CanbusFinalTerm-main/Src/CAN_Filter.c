#include "CAN.h"

void FilterBankConfig(){
    // Vao filter init
    CAN1->FMR |= CAN_FMR_FINIT;

    // Bank 0: 32-bit, MASK mode, FIFO0
    CAN1->FS1R |=  (1U << 0);    // 32-bit scale
    CAN1->FM1R &= ~(1U << 0);    // mask mode
    CAN1->FFA1R &= ~(1U << 0);   // FIFO0

    // ID & MASK (Standard ID ? [31:21])
    uint32_t id  = (0x2U   << 21);
    uint32_t msk = (0x7FFU << 21);
    CAN1->sFilterRegister[0].FR1 = id;
    CAN1->sFilterRegister[0].FR2 = msk;

    // Kich hoat filter
    CAN1->FA1R |= (1U << 0);

    // Thoat filter init
    CAN1->FMR &= ~CAN_FMR_FINIT;
}



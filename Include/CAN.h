// include/can.h
#ifndef CAN_H_
#define CAN_H_

#include "stm32f10x.h" 
#include "CAN_Mode.h"
#include "CAN_Tx.h"
#include "CAN_Rx.h"
#include "CAN_Filter.h"
#include "CAN_errorManage.h"
#include "CAN_timeTrigger.h"
#include <stdio.h>

typedef enum {
    CAN_TX_OK = 0,
    CAN_TX_ERROR,
    CAN_TX_BUSY
} CAN_TxStatus;

CAN_TxStatus CAN_SendFrame(uint16_t stdId, uint8_t *data, uint8_t len);
#endif // CAN_H_


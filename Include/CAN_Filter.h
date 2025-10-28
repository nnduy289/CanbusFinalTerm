#include "CAN.h"

// the bxCAN Controller provides 28 configurable and scalable filter 
//banks (27-0) to the application.
// Each filter bank x consists of two 32-bit registers, CAN_FxR0 and CAN_FxR1 (mask mode or list mode)

void FilterBankConfig();
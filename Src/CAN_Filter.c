#include "CAN.h"

void FilterBankConfig(){
	CAN1->FMR &= ~CAN_FMR_FINIT;
	CAN1->FMR |= CAN_FMR_FINIT; //enter init filter mode
	//filter bank number 0
	CAN1->FS1R &= ~CAN_FS1R_FSC0; //dual 16 bit scale configuration
	CAN1->FM1R |= CAN_FM1R_FBM0;	//Two 32-bit registers of filter bank x are in Identifier List mode
	CAN1->FFA1R &= ~CAN_FFA1R_FFA0; //msg passing this filter will be stored in FIFO0
	
	//config value for filter register to compare (list mode)
	//STID[10:3]\STID[2:0]\RTR\IDE\EXID[17:15]
	//STID = 0x1, RTR = 0, IDE = 0, EXID[17:15] = 0:=> 0x20 TURN ON LED
	//STID = 0x2, RTR = 0, IDE = 0, EXID[17:15] = 0:=> 0x80 TURN OFF LED
	CAN1->sFilterRegister[0].FR1 =  (0x20 << 0) | (0x80 << 16);
	CAN1->FA1R |= CAN_FA1R_FACT0; //active filter 0
}


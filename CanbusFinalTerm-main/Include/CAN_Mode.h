#include "CAN.h"                  // Device header

//==================================================================================================
//======================================Setting MODE for CAN bus====================================
//==================================================================================================

/*FUNCTION: CAN_InitializationModeSetting
*baudrate, filter banks, normal mode, test mode, NART (nonautomatic retransmission), TTCM (time trigger communication mode)...
TTCM: ghi dau thoi gian cuc bo tai moi node (khoang thoi gian giua truyen va nhan)
*Thay doi thong so bit timing, filter, mode phai vao mode Initialization nay
*/
void CAN_InitializationModeSetting();

/*FUNCTION: CAN_NormalModeSetting
*Main state to work: receive, transmit, filter, error detect, interrupt 
*                      synchronize bit timing for other node (time to transfer 1 bit)
*standard ISO 11898
*/
void CAN_NormalModeSetting();

/*FUNCTION: CAN_SleepModeSetting
*Save energy when no need active CAN bus
*
*/
void CAN_SleepModeSetting();

//==================================================================================================
//=============================================TESTING==============================================
//==================================================================================================
/*FUNCTION: CAN_Test_SilentModeSetting
*the bxCAN is able to receive valid data frames and valid remote frames, 
*but it sends only recessive bits on the CAN bus 
*can't transmission to bus, but can receive external event
*used to analyze the traffic on a CAN bus without affecting it by the transimission of dominant bits (ACK Bits, Error Frames)
*suitable for debugging, monitor without affecting to bus
*/
void CAN_Test_SilentModeSetting();

/*FUNCTION: CAN_Test_LoopbackModeSetting
*the bxCAN treats its own transmitted messages as received 
messages and stores them (if they pass acceptance filtering) in a Receive mailbox
*used for selftest functions: performs an internal feedback from its Tx output to its Rx input
*be independent of external event, but can transmit to bus
*suitable for test code with no need physical connect device
*/
void CAN_Test_LoopbackModeSetting();    // 
/*FUNCTION: CAN_Test_HotSelfTest
* silent + loopback
* No affect to bus, can test code while system on running
*/
void CAN_Test_HotSelfTest(); //silent + loopback








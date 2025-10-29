#include "CAN.h"

//CAN Master - loopback mode for testing

volatile uint32_t idDataRcv;
volatile uint32_t dlcTimestampRcv; 
volatile uint32_t lowdataRcv;
volatile uint32_t highdataRcv;

void GPIOconfig(){
	//config PA0 as output pin
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; 	//enable clock for GPIOA
	GPIOA->CRL |= (0x3 << 0); //PA0 as output push-pull 50MHz
}

int main(void){
			//enable clock for CAN bus
			RCC->APB1ENR |= RCC_APB1ENR_CAN1EN;
			//enter testing mode: loopback
			CAN_Test_LoopbackModeSetting();
			while(1){
					TransmitMailboxConfig();
					RecieveMailboxConfig(idDataRcv, dlcTimestampRcv, lowdataRcv, highdataRcv);
					//check 11 bit identifier id 
					if((idDataRcv & 0x8FF) == 0x1){
						//turn on LED PA0
						GPIOA->ODR |= GPIO_ODR_ODR0;
					}else{
						//turn off led PA0
						GPIOA->ODR &= ~GPIO_ODR_ODR0;
					}
			}
			
}
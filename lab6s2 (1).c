#include<LPC17xx.h>
unsigned long LED=0x00000010;
int main(void){
	SystemInit();
	SystemCoreClockUpdate();
	LPC_PINCON->PINSEL0&=0xFF0000FF;
	LPC_GPIO0->FIODIR|=0x00000FF0;
	while(1){
		if(LPC_GPIO2->FIOPIN&(1<<12)){
			LED=0xFFFFFFFF;
			LPC_GPIO0->FIOPIN=LED;
		}
		else{
			LED=0x0;
			LPC_GPIO0->FIOPIN=LED;
		}
	}
}
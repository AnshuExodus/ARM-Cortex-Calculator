#include<LPC17xx.h>
unsigned int i,j;
unsigned long LED = 0x00000010;
int main(void)
{
	SystemInit();
	SystemCoreClockUpdate();
	LPC_PINCON->PINSEL0 &= 0xFF0000FF;
	LPC_GPIO0->FIODIR |= 0x00000FF0;
	while(1)
	{
		LED = 0x00000010;
		while(1)
		{
		if(!(LPC_GPIO2->FIOPIN &(1<<12)))
		{
				LPC_GPIO0->FIOPIN = LED;
				for(j=1;j<100000;j++);
				LED-=0x10;
				if(LED==0x0)
					LED=0xFF0;
			}
		else if(LPC_GPIO2->FIOPIN &(1<<12))
		{
				LPC_GPIO0->FIOPIN = LED;
				for(j=1;j<100000;j++);
				LED+=0x10;
				if(LED==0xFF0)
					LED=0x0;
			}
		}
		}
	}
#include<LPC17xx.h>
unsigned int i=0,j;
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
				if(!(LPC_GPIO2->FIOPIN&(1<<12)))
				{
							
							for(j=1;j<100000;j++);
							if(LED==0x1000){
									LED=0x00000010;
							}
							
							LPC_GPIO0->FIOPIN=LED;
							LED=LED<<1;
				}
				
			}
		}
	}		
	
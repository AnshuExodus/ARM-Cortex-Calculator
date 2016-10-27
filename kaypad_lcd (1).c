//P0.4-P0.11 for Keypad
//P0.23-PO.28 for LCD
#include<LPC17xx.h>
#include<stdio.h>
#define rs_ctrl  0x08000000  //P0.27
#define en_ctrl  0x10000000  //P0.28
#define dt_ctrl  0x07800000  //P0.23 to P0.26 data lines
void lcd_init(void);
void display(void);
void wr_cn(void);
void wr_dn(void);
void clear_ports(void);
void delay(unsigned int);
void lcd_com(void);
void lcd_data(void);
void lcd_puts(void);
void clr_disp(void);
int col, row;
unsigned long int temp,temp1,temp2,temp3,i,o,flag;
unsigned int array_values[4][4]={0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x41,0x42,0x43,0x44,0x45,0x46};
void scan(void);
void  EINT3_IRQHandler(void);
int main()
{
	SystemInit();
	SystemCoreClockUpdate();
	lcd_init();
	LPC_GPIO0->FIOMASK = 0XFFFFF00F;
	LPC_PINCON->PINSEL0&=~(0xFF<<4);
	LPC_GPIO0->FIODIR |= 0xF<<8;
	LPC_GPIO0->FIOPIN |= (0xF<<8);
  NVIC_EnableIRQ(EINT3_IRQn);
	LPC_GPIOINT->IO0IntEnR = (0xF<<4);

while(1);
}

void lcd_init()
{
	LPC_PINCON->PINSEL1 &=0xFC003FFF;
	LPC_GPIO0->FIOMASK = 0xE07FFFFF;
	LPC_GPIO0->FIODIR |= dt_ctrl;
	LPC_GPIO0->FIODIR |= rs_ctrl;
	LPC_GPIO0->FIODIR |= en_ctrl;
	clear_ports();
delay(3200);

temp2=(0x30<<19);
wr_cn();
delay(3000);

temp2=(0x30<<19);
wr_cn();
delay(3000);

temp2=(0x30<<19);
wr_cn();
delay(3000);

temp2=(0x20<<19);
wr_cn();
delay(1000);

temp1=(0x28);
lcd_com();
delay(3000);

temp1=(0x0c);
lcd_com();
delay(800);

temp1=(0x06);
lcd_com();
delay(800);

temp1=(0x01);
lcd_com();
delay(1000);

temp1=(0x80);
lcd_com();
delay(800);
return;
}
void  EINT3_IRQHandler(void)
{
		for(i=0;i<1000;i++);
		//temp3 = LPC_GPIOINT->IO0IntStatR;
		LPC_GPIOINT->IO0IntClr |= temp3;
		LPC_GPIO0->FIOMASK = 0XFFFFF0FF;
		LPC_GPIO0->FIOCLR = 0xF00; 
		for(row=0;row<4;row++)
		{
				if(row==0)
					temp = 0x00000100;		//P0.8
				else if(row==1)
					temp = 0x00000200;		//P0.9
				else if(row==2)
					temp = 0x00000400;		//P0.10
				else if(row==3)
					temp = 0x00000800;		//P0.11
			LPC_GPIO0->FIOMASK = 0XFFFFF0FF;		
			LPC_GPIO0->FIOPIN = temp;
//P0.4-P0.7
	 //temp3 = LPC_GPIO0->FIOPIN;
					flag = 0;
					scan();
	 temp3 &= 0x000000F0;
				if(flag == 1)
						{
					display();
					//LPC_GPIOINT->IO0IntClr = (0xF<<4);
					break;
			}
		}	
		LPC_GPIO0->FIOMASK = 0xFFFFF0FF;
		LPC_GPIO0->FIOPIN = 0xF00;
}
void scan(void)
{
	LPC_GPIO0->FIOMASK = 0XFFFFFF0F;
	temp3 = LPC_GPIO0->FIOPIN;
	temp3 &= 0xF0;
	 if(temp3 != 0x00000000)
	 {
		 flag = 1;
		  if (temp3 ==0x00000010)
				col=0;
			else if (temp3==0x00000020)
				col=1;
			else if (temp3==0x00000040)
				col=2;
			else if (temp3==0x00000080)
				col=3;
		}
}
void lcd_com()
{
	LPC_GPIO0->FIOMASK = 0xE07FFFFF;
	temp2=temp1&0Xf0;
	temp2=temp2<<19;
	wr_cn();
	temp2=temp1&0x0f;
	temp2=temp2<<23;
	wr_cn();
	delay(1000);
	return;
}
void lcd_data()
{
	LPC_GPIO0->FIOMASK = 0xE07FFFFF;
	temp2=temp1&0Xf0;
	temp2=temp2<<19;
	wr_dn();
	temp2=temp1&0x0f;
	temp2=temp2<<23;
	wr_dn();
	delay(1000);
	return;
}

void wr_cn()
{
	LPC_GPIO0->FIOMASK = 0xE07FFFFF;
	clear_ports();
	LPC_GPIO0->FIOPIN=temp2;
	LPC_GPIO0->FIOCLR=rs_ctrl;
	LPC_GPIO0->FIOSET=en_ctrl;
	delay(25);
	LPC_GPIO0->FIOCLR=en_ctrl;
	return;
}


void wr_dn()
{
	LPC_GPIO0->FIOMASK = 0xE07FFFFF;
	clear_ports();
	LPC_GPIO0->FIOPIN=temp2;
	LPC_GPIO0->FIOSET=rs_ctrl;
	LPC_GPIO0->FIOSET=en_ctrl;
	delay(25);
	LPC_GPIO0->FIOCLR=en_ctrl;
	return;
}

void clear_ports(void)

{
		 LPC_GPIO0->FIOMASK = 0xE07FFFFF;	
     LPC_GPIO0->FIOCLR = dt_ctrl;
     LPC_GPIO0->FIOCLR = rs_ctrl;
     LPC_GPIO0->FIOCLR = en_ctrl; 
		 return;
}

void clr_disp(void)

{
		 LPC_GPIO0->FIOMASK = 0xE07FFFFF;	
     temp1 = 0x01;
     lcd_com();
     delay(300);
     return;

}

void delay(unsigned int f)

{
		
   unsigned int r;

   for(r=0;r<f;r++);

   return;

}


void display(void)
{
	LPC_GPIO0->FIOMASK = 0xE07FFFFF;
	temp1=0x80;
	lcd_com();
	delay(200);
	lcd_puts();
//	clr_disp();
}
 
void lcd_puts()
{

		LPC_GPIO0->FIOMASK = 0xE07FFFFF;		
		o = array_values[row][col];
		temp1=o;
	//temp1=0X31;
		lcd_data();	
}

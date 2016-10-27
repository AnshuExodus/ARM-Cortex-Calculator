#include<LPC17xx.h>
#include<stdlib.h>
#define rs_ctrl 0x08000000
#define en_ctrl 0x10000000
#define dt_ctrl 0x07800000

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
void EINT2_IRQHandler(void)
{
display();
	delay(100);
	clr_disp();
	LPC_SC->EXTINT=0x04;
}


unsigned long int temp1,temp2,i;
int main()
{
SystemInit();
	SystemCoreClockUpdate();
	lcd_init();
	LPC_PINCON->PINSEL4 =0x01000000;
	//LPC_PINCON->PINSEL3 &=0x00000000;
	LPC_SC->EXTINT=0x04;
		LPC_SC->EXTMODE=0x04;
	LPC_SC->EXTPOLAR=0x04;
	NVIC_EnableIRQ(EINT2_IRQn);
	while(1);
}

void lcd_init()
{
LPC_PINCON->PINSEL1 &=0xFC003FFF;
	LPC_GPIO0->FIODIR |= dt_ctrl;
LPC_GPIO0->FIODIR |= rs_ctrl;
LPC_GPIO0->FIODIR |= en_ctrl;
clear_ports();
delay(3200);

temp2=(0x30<<19);
wr_cn();
delay(30000);

temp2=(0x30<<19);
wr_cn();
delay(30000);

temp2=(0x30<<19);
wr_cn();
delay(30000);

temp2=(0x20<<19);
wr_cn();
delay(30000);

temp1=(0x28);
lcd_com();
delay(30000);

temp1=(0x0c);
lcd_com();
delay(800);

temp1=(0x06);
lcd_com();
delay(800);

temp1=(0x01);
lcd_com();
delay(10000);

temp1=(0x80);
lcd_com();
delay(800);
return;
}

void lcd_com()
{
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
temp2=temp1&0Xf0;
	temp2=temp2<<19;
	wr_dn();
	temp2=temp1&0x0f;
	temp2=temp2<<23;
	wr_dn();
	delay(10000000);
	return;
}

void wr_cn()
{
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
     LPC_GPIO0->FIOCLR = dt_ctrl;

     LPC_GPIO0->FIOCLR = rs_ctrl; 

     LPC_GPIO0->FIOCLR = en_ctrl; 

      

   return;

}

void clr_disp(void)

{

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
temp1=0x80;
	lcd_com();
	delay(200);
	lcd_puts();
//	clr_disp();
	
}

void lcd_puts()
{
unsigned int o=rand();
	o=o%7;
	if(o==0)
		o=1;
	temp1=o+0X30;
	//temp1=0X31;
	lcd_data();	
}
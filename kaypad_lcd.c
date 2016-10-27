
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
unsigned int array_values[4][4]={0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F};
void scan(void);


int main()
{
SystemInit();
	SystemCoreClockUpdate();
	lcd_init();
	LPC_GPIO0->FIOMASK&= 0XFF<<4;
	LPC_PINCON->PINSEL0&=~(0xFF<<4);
	LPC_GPIO0->FIODIR |= 0xF<<8;


	while(1)
	{
	
for(row=0;row<4;row++)
			{
				if(row == 0)
				temp = 0x00000100;
				else if(row == 1)
				temp = 0x00000200;
				else if(row == 2)
				temp = 0x00000400;
				else if(row == 3)
				temp = 0x00000800;
			
				LPC_GPIO0->FIOPIN = temp;
				flag = 0;
				scan();
				if(flag == 1)
				{
				
			  o = array_values[row][col];
				break;
				}
			} // end of second for loop
		//end of first for loop
			temp1=o+'0';
display();
	delay(100);
	clr_disp();
}
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
void scan(void)
 {
  LPC_PINCON->PINSEL0=0X0F<<4;
	 LPC_GPIO0->FIOMASK |= ~(0X0F<<4);
	 LPC_GPIO0->FIODIR = ~(0X0F<<4);
	temp3 = LPC_GPIO0->FIOPIN;	
	temp3 &= 0x000000F0;
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
   
	}//1st if(temp3 != 0x00000000)
 }//end scan
 
void lcd_puts()
{

			
	temp1=o+'0';
	//temp1=0X31;
	lcd_data();	
}

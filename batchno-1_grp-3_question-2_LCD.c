/**
BATCH 3 
CCE B1 

Ojas Agashe - 140953028
Ravi Teja- 140953045
Sukriti Paul- 140953036

Display the BCD sum,using LCD Interfacing and interrupts
**/


//P0.4-P0.11 for Keypad
//P0.23-PO.28 for LCD
#include<LPC17xx.h>
#include<stdlib.h>
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
void char_puts(char ch);//not used
void str_puts(char *buf1);


void clr_disp(void);
int col, row;
unsigned long int temp,temp1,temp2,temp3,i=0,o,flag,wait,n,brk=0,ind,loop=0,first=1,d;
int result=0;
char cnum[4],ans[5],num[4];

unsigned int row_val[4]={0x100,0x200,0x400,0x800};
unsigned int data[4][4]={'0','1','2','3','4','5','6','7','8','9','c','k'};
void scan(void);
void  EINT3_IRQHandler(void);
 


int main()
{
	SystemInit();
	SystemCoreClockUpdate();
	lcd_init();
	LPC_GPIO0->FIOMASK = 0XFFFFF00F;
	LPC_PINCON->PINSEL0&=~(0xFF<<4);           //gpio
	LPC_GPIO0->FIODIR |= 0xF<<8;               //rows p0.8 - p0.11 as output
	LPC_GPIO0->FIOPIN |= (0xF<<8);             //all rows are set to 1
  str_puts("Enter value of n");
	NVIC_EnableIRQ(EINT3_IRQn);
	LPC_GPIOINT->IO0IntEnR = (0xF<<4);        //columns enabled as interrupts



while(1)
{
	if(brk==1)
	{ 
	  first=0;
		break;
  }
}

brk=0;
clr_disp();
delay(900);

while(loop<n) //n is the number of BCD numbers
{
  while(1)
  {
    if(brk==1) //Flag is set when user enters the OK key
    break;
  }
  loop++;
  brk=0;
	clr_disp();
	temp1=(0x80);
  lcd_com();
  delay(800);
}

str_puts("SUM:");
d=sprintf(ans,"%d%",result);   //get string value of result
str_puts(ans);
}


void  EINT3_IRQHandler(void)
{
		for(i=0;i<1000;i++);                 //for debounce

		temp3 = LPC_GPIOINT->IO0IntStatR;
		
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

			flag = 0;
				     scan();
	 temp3 &= 0x000000F0;
				
	if(flag==1)			
				
	{	if(data[row][col]=='k')
				{
					 brk=1;
					 ind=0;
				if(!first) 
				{	 result+=atoi(num);
				}
				else
				{
				  first=0;
	        n=atoi(num);
					
					
				}	
        for (i=0;i<4;i++)
        {
	         num[i]=' ';
        }				
    LPC_GPIO0->FIOMASK = 0xFFFFF0FF;       //ROWS SET TO 1 AGAIN
		LPC_GPIO0->FIOPIN = 0xF00;
	  LPC_GPIOINT->IO0IntClr=0xF0;
    for(wait=0;wait<35000;wait++);  				
				clr_disp();	
    return;				
				}
     else if (data[row][col]=='c')
			{
         ind--;        
				 num[ind]=' ';
				clr_disp();
          str_puts(num);
				 break;
			}						
		else
		{			
		
				
					num[ind]=data[row][col];
					str_puts(num);
					ind++;
					break;
			  
		}
  }		
	}
		LPC_GPIO0->FIOMASK = 0xFFFFF0FF;       //ROWS SET TO 1 AGAIN
		LPC_GPIO0->FIOPIN = 0xF00;
	  LPC_GPIOINT->IO0IntClr=0xF0;
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



void display(void)
{
	  LPC_GPIO0->FIOMASK = 0xE07FFFFF;
   	delay(200);
  	char_puts(data[row][col]);
//	clr_disp();
}



 void str_puts(char *buf1)
 {
	  clr_disp();                  //clear previous display
    i=0;

    while(buf1[i]!='\0')
    {
        temp1 = buf1[i];
     	lcd_data();
		i++;
        if(i==16)
		{
           	temp1 = 0xc0;
			lcd_com();
		}
         
       }
    for(wait=0;wait<35000;wait++);    
    return;
 }



//-----------------------LCD CODE-----------------------------------------------------------------------------------------


void char_puts(char ch)
{

		LPC_GPIO0->FIOMASK = 0xE07FFFFF;		
		o = ch;
		temp1=o;
		lcd_data();	
	 for(wait=0;wait<25000;wait++);
}




  
 
 

 void lcd_init()
{
	LPC_PINCON->PINSEL1 &=0xFC003FFF;        //P0.23 - P0.28 io lines
 	LPC_GPIO0->FIOMASK = 0xE07FFFFF;         //mask all except P0.23 - P0.28 
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



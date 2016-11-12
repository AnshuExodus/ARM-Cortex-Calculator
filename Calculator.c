#include <LPC17xx.h>
#include <math.h>

#define RS_CTRL  0x08000000  
#define EN_CTRL  0x10000000  
#define DT_CTRL  0x07800000  

#define PI 3.1415

void scan(void);
void lcd_init(void);
void wr_cn(void);
void clr_disp(void);
void delay_lcd(unsigned int);
void lcd_com(void);						   
void wr_dn(void);
void lcd_data(void);
void clear_ports(void);
void lcd_puts(unsigned char *);
void chkOpRand();


void delay()
{
	int i;
	for(i=0; i<50000; i++);
}
void smallDelay()
{
	int i;
	for(i=0; i<500; i++);
}

void clearPorts()
{
	LPC_GPIO0->FIOCLR=3<<27;
	LPC_GPIO0->FIOCLR=0xf<<23;
}


 int higherNibble(int x)
{
	return x & 0xF0;
}
int lowerNibble(int x)
{
	return x & 0x0F;
}

void enableWrite()
{
	LPC_GPIO0->FIOSET=1<<28;
	smallDelay();
	LPC_GPIO0->FIOCLR=1<<28;
	smallDelay();
}

 void LCD_writeData(int x)
{
	clearPorts();
	LPC_GPIO0->FIOPIN=higherNibble(x)<<19;
	LPC_GPIO0->FIOSET=1<<27;	//RS=1
	enableWrite();
	
	clearPorts();
	LPC_GPIO0->FIOPIN=lowerNibble(x)<<23;
	LPC_GPIO0->FIOSET=1<<27;	//RS=1
	enableWrite();
	
	delay();
}

void LCD_writeCommand(int x)
{
	clearPorts();
	LPC_GPIO0->FIOPIN=higherNibble(x)<<19;
	LPC_GPIO0->FIOCLR=1<<27;	//RS=0
	enableWrite();
	
	clearPorts();
	LPC_GPIO0->FIOPIN=lowerNibble(x)<<23;
	LPC_GPIO0->FIOCLR=1<<27;	//RS=0
	enableWrite();
	
	delay();
}

unsigned char col,row,flag;
unsigned long int i,var1,temp,temp3,k, eq = 0; 
unsigned long int temp1=0, temp2=0 ;
unsigned char tempz[16];
unsigned long int op, fo = 0, dec = 0;
float op1, op2, a = 0.0;
long int pow10 = -1;
int size;
unsigned char ans[10];
int mode = 0;
 
 

unsigned char seven_code[4][4]={{0x30,0x31,0x32,0x33}, {0x34,0x35,0x36,0x37}, {0x38,0x39,0x2E,0}, {0x2B,0x2D,0x2A,0x2F}};
	int main(void)
 {
	SystemInit();
	SystemCoreClockUpdate();
  
	 
	 LPC_GPIO2->FIODIRL=0x3C00;
	 
  lcd_init();				
	delay_lcd(3200);

	temp1 = 0x80;			
	lcd_com();
	delay_lcd(800);
		while(1)
		{
		 for(row = 0; row<4; row++)
		 {
			 temp = (1<<(row+10));
			 LPC_GPIO2->FIOPIN = temp;
			 flag= 0;
			 scan();
			 if(flag == 1)
			 {
				chkOpRand();
				LCD_writeCommand(temp1);
				tempz[k++]=seven_code[row][col];
				 
				if (!eq && !mode)
				{
					LCD_writeData(tempz[k-1]);
					temp1++;
					eq = 0;
				}
				
				for(i=0;i<50000;i++);
				clear_ports();
				
				if (temp1 >= 0x8F && temp1 < 0xC0)
					temp1 = 0xC0;
				if (temp1 >= 0xCF)
					temp1 = 0x80;
				lcd_com();
				delay_lcd(800);
				break;	 
			 }
		 }
	 }
 }

void disp()
{
		int lineVal = 0x81, tem, cou = 0, point;
		float p1, p2;
		float afterPoint;
		LCD_writeCommand(0x01);
		LCD_writeCommand(0x80);
	if (op == 0)	
		a = (op1+op2);
	else if(op == 1)
		a = (op1-op2);
	else if(op == 2)
		a = (op1*op2);
	else if(op == 3)
		a = (op1/op2);
	else if (op == 4)
		a = pow(op1, op2);
	else if (op == 5)
		a = sin(op2);
	else if (op == 6)
		a = cos(op2);
	else if (op == 7)
		a = tan(op2);
	else if (op == 8)
		a = PI;
	else if (op == 9)
		{a = log(op2);}
		//a*=2;
	else if(op == 10)
		a=pow(op2,10);
	
		op1 = a;
		op2 = 0;
		i = 0;
		size = 0;
		if (a < 0)
		{
				LCD_writeData(0x2D);
				LCD_writeCommand(lineVal++);
			a *= -1;
		}
		if (a == 0)
		{
			LCD_writeData(0x30);
			LCD_writeCommand(lineVal++);
		}
		tem = (int)a;
		if (tem == a)
		{			
		while(tem)
		{
				ans[size] = ((tem%10)+0x30);
				tem /= 10;
				size++;
		}
		//ans[size++] = 0x25;
		size--;
		while (size >= 0)
		{
				LCD_writeData(ans[size]);
				LCD_writeCommand(lineVal++);
				size--;
		}
		LCD_writeData(0x2E);	
		LCD_writeCommand(lineVal++);

		}
		else
		{
				afterPoint = a-tem; 
				afterPoint *= 10;
				while(tem)
				{
				ans[size] = ((tem%10)+0x30);
				tem /= 10;
				size++;
				}
				point = size;
				ans[size] = '.';
				size++;
					
				while((int)afterPoint)
				{
					ans[size++] = ((int)afterPoint+0x30);
					afterPoint -= (int)afterPoint;
					afterPoint *= 10;
				}
				
				cou = point-1;
				while (cou >= 0)
				{
					LCD_writeData(ans[cou--]);
					LCD_writeCommand(lineVal++);
				}
				
				LCD_writeData(0x2E);	
				LCD_writeCommand(lineVal++);
				point++;
				while (point < size)
				{
						LCD_writeData(ans[point++]);
						LCD_writeCommand(lineVal++);
				}
				LCD_writeData(0x30);	
				LCD_writeCommand(lineVal++);

		}
		
		temp1 = lineVal;
}
void chkOpRand()
{ int del = 10000;
	if (row == 3 && col == 3)
	{
			if (mode) mode = 0;
			else { mode = 1; fo = 1;}
			temp1++;
			LCD_writeData('M');
			LCD_writeCommand(temp1++);
			eq = 1;
	}
	else if (!mode)
	{
	if (row == 3)
	{	
		flag = 1;
		fo = 1;
		op = col;
		if (op == 0)
		{
				LCD_writeData('+');
				LCD_writeCommand(temp1++);
		}
		else if(op == 1)
		{
				LCD_writeData('-');
				LCD_writeCommand(temp1++);
		}
		else if(op == 2)
		{
				LCD_writeData('*');
				LCD_writeCommand(temp1++);
		}
		eq = 1;
		dec = 0; 
	}
	else if (row == 2 && col == 2)
	{
			dec = 1; pow10 = -1;
	}
	else if ((row == 2)&&(col == 3))
	{
		k = 0;
		eq = 1;
		disp();
	}
	else if (!fo)
	{ eq = 0;
		if (!dec) op1 = (seven_code[row][col]-0x30)+(op1*10);
		else {
			op1 += (seven_code[row][col]-0x30)*pow(10,pow10);
			pow10--;
		}
	}
	else if (fo)
	{ eq = 0;
		if (!dec) op2 = (seven_code[row][col]-0x30)+(op2*10);
		else {
			op2 += (seven_code[row][col]-0x30)*pow(10,pow10);
			pow10--;
		}
	}
	}
	else {
			if (row == 0 && col == 0)
			{		op = 3;
				LCD_writeData('/');
					LCD_writeCommand(temp1++);
			}
			else if (row == 0 && col == 1)
			{		op = 4;
					LCD_writeData('^');
					LCD_writeCommand(temp1++);
			}
			else if (row == 0 && col == 2)
			{
					op = 5;
					LCD_writeCommand(0x01);
				  temp1 = 0x80;
					LCD_writeCommand(temp1++);
					LCD_writeData('s');
					LCD_writeCommand(temp1++);
					LCD_writeData('i');
					LCD_writeCommand(temp1++);
					LCD_writeData('n');
					LCD_writeCommand(temp1++);
			}
			else if (row == 0 && col == 3)
			{
					op = 6;
				  LCD_writeCommand(0x01);
				  temp1 = 0x80;
					LCD_writeCommand(temp1++);
					LCD_writeData('c');
					LCD_writeCommand(temp1++);
					LCD_writeData('o');
					LCD_writeCommand(temp1++);
					LCD_writeData('s');
					LCD_writeCommand(temp1++);
			}
			else if (row == 1 && col == 0)
			{
					op = 7;
				  LCD_writeCommand(0x01);
				  temp1 = 0x80;
					LCD_writeCommand(temp1++);
					LCD_writeData('t');
					LCD_writeCommand(temp1++);
					LCD_writeData('a');
					LCD_writeCommand(temp1++);
					LCD_writeData('n');
					LCD_writeCommand(temp1++);
			}
			else if (row == 1 && col == 1)
			{
					op = 8;
				  LCD_writeCommand(0x01);
				  temp1 = 0x80;
					LCD_writeCommand(temp1++);
					LCD_writeData('p');
					LCD_writeCommand(temp1++);
					LCD_writeData('i');
					LCD_writeCommand(temp1++);
					LCD_writeData('e');
					LCD_writeCommand(temp1++);
			}
			else if (row == 1 && col == 2)
			{
					op = 9;
				  LCD_writeCommand(0x01);
				  temp1 = 0x80;
					LCD_writeCommand(temp1++);
					LCD_writeData('l');
					LCD_writeCommand(temp1++);
					LCD_writeData('o');
					LCD_writeCommand(temp1++);
					LCD_writeData('g');
					LCD_writeCommand(temp1++);
			}
			else if (row == 1 && col == 3)
			{
					op = 10;
				  LCD_writeCommand(0x01);
				  temp1 = 0x80;
					LCD_writeCommand(temp1++);
					LCD_writeData('p');
					LCD_writeCommand(temp1++);
					LCD_writeData('o');
					LCD_writeCommand(temp1++);
					LCD_writeData('w');
					LCD_writeCommand(temp1++);
			}
	}

}	

 

 void lcd_init()
 {
	
    LPC_PINCON->PINSEL1 &= 0xFC003FFF;  

	
    LPC_GPIO0->FIODIR |= DT_CTRL;	
		LPC_GPIO0->FIODIR |= RS_CTRL;	
		LPC_GPIO0->FIODIR |= EN_CTRL;	
        
    clear_ports();
	delay_lcd(3200);

	temp2 = (0x30<<19);
	wr_cn();	   
	delay_lcd(30000); 
		
	temp2 = (0x30<<19);
	wr_cn();
	delay_lcd(30000);	 
		
	temp2 = (0x30<<19);
	wr_cn();
	delay_lcd(30000);

	temp2 = (0x20<<19);
	wr_cn();
	delay_lcd(30000);

	temp1 = 0x28;
	lcd_com();
	delay_lcd(30000);
		
	temp1 = 0x0C;		
	lcd_com();
	delay_lcd(800);
	
	temp1 = 0x06;
	lcd_com();
	delay_lcd(800);
	
	temp1 = 0x01;
	lcd_com();
 	delay_lcd(10000);
	
	temp1 = 0x80;
	lcd_com();
	delay_lcd(800);
    return;
 }
 
  void scan(void)
 {
	 temp3=LPC_GPIO1->FIOPIN;
	 temp3=temp3&0x07800000;
	 if(temp3!=0x0)
	 {
		 flag=1;
		 if(temp3==0x00800000)
			 col=0;
		 else if(temp3==0x01000000)
			 col=1;
		 else if(temp3==0x02000000)
			 col=2;
		 else if(temp3==0x04000000)
			 col=3;
	 }
 }

 void lcd_com(void)
 {
	temp2 = temp1 & 0xf0;
	temp2 = temp2 << 19;
	wr_cn();
	temp2 = temp1 & 0x0f; 
	temp2 = temp2 << 23; 
	wr_cn();
	delay_lcd(1000);
    return;
 }

 void wr_cn(void)                        
 { 	 
	clear_ports();
	LPC_GPIO0->FIOPIN = temp2;		
  LPC_GPIO0->FIOCLR = RS_CTRL;		
	LPC_GPIO0->FIOSET = EN_CTRL;   	
	delay_lcd(25);
	LPC_GPIO0->FIOCLR = EN_CTRL;		 		
    return;
    
 }
 
 void lcd_data(void)
 {             
    temp2 = temp1 & 0xf0;
    temp2 = temp2 << 19;
    wr_dn();
    temp2= temp1 & 0x0f;	
    temp2= temp2 << 23;
    wr_dn();
    delay_lcd(1000);	
    return;
 } 

 void wr_dn(void)
 {  	  
	clear_ports();

	LPC_GPIO0->FIOPIN = temp2;	   
	LPC_GPIO0->FIOSET = RS_CTRL;
	LPC_GPIO0->FIOSET = EN_CTRL;   
	delay_lcd(25);
	LPC_GPIO0->FIOCLR = EN_CTRL;
    return;
 }

 void delay_lcd(unsigned int r1)
 {
  	unsigned int r;
  	for(r=0;r<r1;r++){
			;
		}
    return;
 }

 void clr_disp(void)
 {
	temp1 = 0x01;
	lcd_com();
 	delay_lcd(10000);
    return;
 }
 void clear_ports(void)
 {
	LPC_GPIO0->FIOCLR = DT_CTRL;
	LPC_GPIO0->FIOCLR = RS_CTRL;
	LPC_GPIO0->FIOCLR = EN_CTRL;
        
    return;
 }

 void lcd_puts(unsigned char *buf1)
 {
     unsigned int i=0;

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
    return;
 }

 
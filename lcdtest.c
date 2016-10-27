/*
CCE A2
EMBEDDED SYSTEMS PROJECT

RISHIKESH KULKARNI
GEORGE MAMMEN JACOB
AYUSH VERMA


*/

#include<LPC17xx.h>
#include<stdio.h>
#include<stdlib.h>
#define RS_CTRL 0x08000000 //P0.27
#define EN_CTRL 0x10000000 //P0.28
#define DT_CTRL 0x07800000 //P0.23 to P0.26 data lines
#define MAX 10
#define MASK_ROWS 0XF00; 	//P0.8 to P0.11 are connected to rows
#define MASK_COLS 0XF0;		//P0.4 to P0.7 are connected to columns
#define MASK_DISP 0X1F800000; //LCD Display is from P0.23 to P0.28




void lcd_init(void);
void wr_cn(void);
void clr_disp(void);
void delay_lcd(unsigned int);
void lcd_com(void);
void wr_dn(void);
void lcd_data(void);
void clear_ports(void);
void lcd_puts(unsigned char *);

void init_IOInt(void);
void init_ports(void);
void LCD_DATA1(void);
void LCD_ACCEPT(void);
void LCD_CLEAR(void); //not used
void LCD_FINAL(void);
void display(void);


unsigned long int temp1=0, temp2=0 ;
int arr[4][4]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
int final_val,num,wait;
unsigned int temp3,temp4,row,col,flag,j,i;
int temp,pos,count;
int sum[MAX];
unsigned long var;
unsigned char val[16];
unsigned char Msg[4]={"SUM"};

int main(void)
{
	SystemInit();
	SystemCoreClockUpdate();


	init_ports();
	init_IOInt();

	LPC_GPIO0->FIOMASK=0XFF0;
	lcd_init();
	LPC_GPIO0->FIOMASK=0X0;

	temp=num=count=0;
	pos=1;
	final_val=0;

	temp1=0x80;
	lcd_com();
	delay_lcd(800);

	NVIC_EnableIRQ(EINT3_IRQn);


   LPC_GPIO0->FIOMASK= 0X1F800000;
	 LPC_GPIO0->FIOPIN= MASK_ROWS; //Set All rows as 1
	 LPC_GPIO0->FIOMASK=0X0;
	while(1); //Poll till interrupt is generated
}
void display(void)
{

	temp=arr[row][col]; //Gets corresponding value from array

	LPC_GPIO0->FIOMASK=0XFF0; // Mask the Row & Column pins so that no change is brought on to them while working with the lcd

	if((temp>=0)&&(temp<10)) //Digits that need to be displayed and added to the current number
		{
		num=(int)(num*10)+(int)temp;

		if(num!=0)
		pos=pos*10;

		LCD_DATA1();
		for(j=0;j<3000;j++);
		}
	else if(temp==10) //To accept the number entered
		{
		sum[count]=num;
		count++;
		num=0;
		pos=1;
		LCD_ACCEPT();
		for(j=0;j<3000;j++);
		}
	else if(temp==11) //To make a correction to the displayed number
		{
		if(pos==1)
			num=0;
		else
		{
		num=num/10;
		pos=pos/10;
		}

		LCD_DATA1();
		for(j=0;j<3000;j++);
		}
	if(temp==12||MAX==count) //Get and display the final sum
		{
		for(i=0;i<count;i++)
		final_val=final_val+sum[i];

			count=0;
		num=0;
		pos=1;

		LCD_FINAL();
		for(j=0;j<35000;j++);

		final_val=0;
		}
	LPC_GPIO0->FIOMASK=0X0;
	flag=0;
	for(i=0;i<50;i++);
	}


void EINT3_IRQHandler(void)
	{
		for(wait=0;wait<1000;wait++); //DEBOUNCE

		temp3=LPC_GPIOINT->IO0IntStatR;
		temp3=temp3&MASK_COLS;
		temp3=temp3>>4;

		if(temp3==0x1)
			col=0;
		else if(temp3==0x2)
			col=1;
		else if(temp3==0x4)
			col=2;
		else if(temp3==0x8)
			col=3;


		/*TO GET ROW VALUE*/
		while(flag==0)
		{
		for(i=0;i<4;i++)
		{
			if(i==0)
				var=0x1<<8;
			else
				var=var<<1;

			LPC_GPIO0->FIOMASK= 0X1F800000;
			LPC_GPIO0->FIOPIN= var;
			LPC_GPIO0->FIOMASK=0X0;

			for(wait=0;wait<100;wait++);

			temp4=LPC_GPIO0->FIOPIN;
			temp4=temp4&(0xF0); //If a column pin is high, then corresponding the key is in the corresponding Row

			if(temp4!=0x0)
			{
				row=i;
				flag=1;
				break;
			}

		}
	}


		display();
		//Clear row values so that no interrupt is generated until the value is displayed and the controller is ready to get the next value
	 for(j=0;j<16;j++)
		val[j]=0x0;

	 

	 LPC_GPIO0->FIOMASK= 0X1F800000; //mask lcd & Columns
	 LPC_GPIO0->FIOSET= MASK_ROWS;
	 LPC_GPIO0->FIOMASK=0X0;
	
	for(i=0;i<200;i++);

	 LPC_GPIOINT->IO0IntClr|=MASK_COLS;
	}

void init_IOInt()
	{
		LPC_GPIOINT->IO0IntEnR=MASK_COLS; // Enables Rising Edge External Interrupt for Column pins
	}
void init_ports()
	{LPC_GPIO0->FIODIR|= 0X1F800000|0XF00; //sets P0.8 to P0.11 and P0.23 to P0.28 as O/P ports
	}

void LCD_DATA1() //Stores "num" as a string and then displays it
{
 sprintf(val,"%d",num);
 clr_disp();

 temp1 = 0x80;
 lcd_com();
 delay_lcd(800);

 lcd_puts(&val[0]);

}

void LCD_ACCEPT() //clears the screen
{
 clr_disp();

 temp1 = 0x80;
 lcd_com();
 delay_lcd(800);
}

void LCD_FINAL() //To display the final sum
{
	clr_disp();

 temp1 = 0x80;
 lcd_com();
 delay_lcd(800);

 lcd_puts(&Msg[0]);

	temp1=0XC0;
	lcd_com();
	delay_lcd(800);

	sprintf(val,"%d",final_val);
	lcd_puts(&val[0]);
}

void lcd_init()
{
     /* Ports initialized as GPIO */
   LPC_PINCON->PINSEL1 &= 0xFC003FFF; //P0.23 to P0.28

     /* Setting the directions as output */
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

     temp1 = 0x0c;
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

void lcd_com(void)
{
     temp2 = temp1 & 0xf0;//move data (26-8+1) times : 26 - HN
                           //place, 4 - Bits
     temp2 = temp2 << 19; //data lines from 23 to 26
     wr_cn();
     temp2 = temp1 & 0x0f; //26-4+1
     temp2 = temp2 << 23;
     wr_cn();
     delay_lcd(1000);
   return;
}

// command nibble o/p routine
void wr_cn(void)                       //write command reg
{
     clear_ports();
     LPC_GPIO0->FIOPIN = temp2;// Assign the value to the data
                                 //lines
   LPC_GPIO0->FIOCLR = RS_CTRL;           // clear bit RS
     LPC_GPIO0->FIOSET = EN_CTRL;   // EN=1
     delay_lcd(25);
     LPC_GPIO0->FIOCLR = EN_CTRL;   // EN =0
   return;

}

// data o/p routine which also outputs high nibble first
// and lower nibble next
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

// data nibble o/p routine
void wr_dn(void)
{
     clear_ports();

     LPC_GPIO0->FIOPIN = temp2;      // Assign the value to the
                                     //data lines
     LPC_GPIO0->FIOSET = RS_CTRL;    // set bit RS
     LPC_GPIO0->FIOSET = EN_CTRL;   // EN=1
     delay_lcd(25);
     LPC_GPIO0->FIOCLR = EN_CTRL;    // EN =0
   return;
}

void delay_lcd(unsigned int r1)
{
   unsigned int r;
   for(r=0;r<r1;r++);
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
   /* Clearing the lines at power on */
     LPC_GPIO0->FIOCLR = DT_CTRL; //Clearing data lines
     LPC_GPIO0->FIOCLR = RS_CTRL; //Clearing RS line
     LPC_GPIO0->FIOCLR = EN_CTRL; //Clearing Enable line

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
	 //for(i=0;i<30000;i++);
   return;
}

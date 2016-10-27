/*********************************************************************
 * Key pad Interface
 * Developed by
 * Advanced Electronics Systems, Bengaluru
 *--------------------------------------------------------------------
 * 4x4, matrix keyboard has total 16 keys. There are 4 rows and 4 cols
 * Identity of key pressed (0 to F) will be displayed on LCD. 
 * Port lines used : P1.23 to P1.26 - col1 to col4, P2.10 to P2.13
 * row1 to row4. Connect 10 pin FRC cable from CNB to CNB3,
 * Short JP4 (1, 2)
 *********************************************************************/

 #include <LPC17xx.h>
 #define	FIRST_SEG	0xFFF87FFF
 #define  SECOND_SEG 0xFFF8FFFF
 #define  THIRD_SEG 0xFFF97FFF
 #define  FOURTH_SEG 0xFFF9FFFF
 #define  DISABLE_ALL 0xFFFA7FFF
 void scan(void);
 void display1(void);
 void display2(void);
 void delay(void);
 unsigned char col,row,flag,flag1,flag2;
 unsigned char tempi[4],dig=3;
 unsigned long int i,j=0,var1,temp,temp3,temp2;
 unsigned char SEVEN_CODE[4][4] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x73,0x40,0x48};
 int main(void)
 {
 	SystemInit();
	SystemCoreClockUpdate();
	LPC_PINCON->PINSEL0 &= 0xFFFFFFFF;	 //P0.4 to P0.11 GPIO data lines
	LPC_GPIO0->FIODIR |= 0xFFFFFFFF;	//P0.4 to P0.11 output
	LPC_PINCON->PINSEL3 &= 0xFFC03FFF; //P1.23 to P1.26 MADE GPIO
	LPC_PINCON->PINSEL3 &= 0xF00FFFFF; //P2.10 t P2.13 made GPIO
	LPC_GPIO2->FIODIR |= 0x00003C00; //made output P2.10 to P2.13 (rows)
	LPC_GPIO1->FIODIR &= 0xF87FFFFF; //made input P1.23 to P1.26 (cols)

	
	while(1)
	{
		for(row=0;row<4;row++)
			{
				if(row == 0)
				temp = 0x00000400;
				else if(row == 1)
				temp = 0x00000800;
				else if(row == 2)
				temp = 0x00001000;
				else if(row == 3)
				temp = 0x00002000;
			
				LPC_GPIO2->FIOPIN = temp;
				flag = 0;
				scan();
				if(flag == 1)
				{
				display1();
				dig--;
				break;
				}
			} // end of second for loop
		//end of first for loop
			
		}//end while 1

 }//end main

 void scan(void)
 {

	temp3 = LPC_GPIO1->FIOPIN;	
	temp3 &= 0x07800000;
	if(temp3 != 0x00000000)
	{
				flag = 1;
			if (temp3 ==0x00800000)
				col=0;
			else if (temp3==0x01000000)
				col=1;
			else if (temp3==0x02000000)
				col=2;
			else if (temp3==0x04000000)
				col=3;
   
	}//1st if(temp3 != 0x00000000)
 }//end scan
 void display1()
 {
		while(1)
		{
		tempi[j++] = SEVEN_CODE[row][col];
		if(dig==3)
		{
			LPC_GPIO0->FIOMASK = 0xFFF87FFF;
			LPC_GPIO0->FIOPIN = FIRST_SEG;
			tempi[0] = tempi[0]<<4;
			LPC_GPIO0->FIOMASK = 0xFFFFF00F;
			LPC_GPIO0->FIOPIN = tempi[0];
		}
		if(dig==2)
		{
			LPC_GPIO0->FIOMASK = 0xFFF87FFF;
			LPC_GPIO0->FIOPIN = SECOND_SEG;
			tempi[1] = tempi[1]<<4;
			LPC_GPIO0->FIOMASK = 0xFFFFF00F;
			LPC_GPIO0->FIOPIN = tempi[1];
		}
		if(dig==1)
		{
			LPC_GPIO0->FIOMASK = 0xFFF87FFF;
			LPC_GPIO0->FIOPIN = THIRD_SEG;
			tempi[2] = tempi[2]<<4;
			LPC_GPIO0->FIOMASK = 0xFFFFF00F;
			LPC_GPIO0->FIOPIN = tempi[2];
		}
		if(dig==0)
		{
			LPC_GPIO0->FIOMASK = 0xFFF87FFF;
			LPC_GPIO0->FIOPIN = FOURTH_SEG;
			tempi[3] = tempi[3]<<4;
			LPC_GPIO0->FIOMASK = 0xFFFFF00F;
			LPC_GPIO0->FIOPIN = tempi[3];
		}
		delay();
	}
	}
	void delay()
	{
			for(i=0;i<1000;i++);
	}
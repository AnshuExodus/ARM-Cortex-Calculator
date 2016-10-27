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
 
 //#define	FIRST_SEG	0xF87FFFFF
 #define	SECOND_SEG	0xFFF8FFFF
 #define	THIRD_SEG	0xFFF97FFF
 #define	FOURTH_SEG	0xFFF9FFFF
 #define	DISABLE_ALL 0xFFFA7FFF
 void scan(void);
 void disp(void);
 void calc(void);
 void disp1(void);
 unsigned char col,row,flag,n_flag=0;
 unsigned long int i=0,j,var1,temp,temp3,temp2;
 //unsigned char SEVEN_CODE[4] = {0x01,0x22,0x14,0x08};
 unsigned char SEVEN_CODE[10]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
 unsigned int value[4][4] = {0,1,2,3,4,5,6,7,8,9,11,12,0,0,0,13};
	
	int op1, expr, op2, tens,unit, temp4; 
	int arr[4];

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
					calc();
				  break;
				}
			} //end for(row=1;row<5;row++)
			
		}//end while 1

 }//end main

 void scan(void)
 {
 	unsigned long temp3;

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
 
 
 void calc(void)
 {
					

	 
					if(i==0)
					{
					arr[i]=value[row][col];
					temp2=SEVEN_CODE[arr[i]];
						i++;
						disp();
					
						
					}
					else if(i==1)
					{
						
						arr[i]=value[row][col];
						
						
						if(arr[i]==11)
						{
						temp2=0x73;
						disp();
					 }
					 else if(arr[i]==12)
						{
						temp2=0x40;
						disp();
					 }
					 else
					 {
						temp2=0x79;
						disp();
					 }	
							i++;
					}
					else if(i==2)
					{
					arr[i]=value[row][col];
					temp2=SEVEN_CODE[arr[i]];
						disp();
						
						if(arr[i-1]==11)
							arr[++i]=arr[0]+arr[2];
						else if(arr[i-1]==12)
							arr[++i]=arr[0]-arr[2];
					}
					else if(i==3)
					{
						
							if(value[row][col]!=13)
							{
								temp2=0x79;
								disp();
								}				
							else
							{
						
						//temp2=SEVEN_CODE[arr[i]];
						//disp();
						//i=0;
							if(arr[i]>0)
							{
									unit=arr[i]%10;
									tens=arr[i]/10;
									disp1();
							}
							else
							{
								n_flag=1;
								arr[i]*=-1;
								unit=arr[i]%10;
								tens=arr[i]/10;
								disp1();
								}
		
							}
						}
}


void disp()
{				
	 
					
					//LPC_GPIO0->FIOPIN=0xFFF87FFF;
						LPC_GPIO0->FIOMASK=0xFFF87FFF;
					
	 

							LPC_GPIO0->FIOPIN=FIRST_SEG;
	 
	 
					temp2=temp2<<4;
					LPC_GPIO0->FIOMASK=0xFFFFF00F;
					LPC_GPIO0->FIOPIN=temp2;
	
						for(j=0; j<50000;j++);
						for(j=0; j<50000;j++);
						
					
}

void disp1(void)
{
	int flag=0;
		while(1)
		{
			
			if(n_flag==0)
			{
			LPC_GPIO0->FIOMASK=0xFFF87FFF;
       if(flag==0)		// For Segment U8
	    {
		   temp2 = SEVEN_CODE[unit]; 
		   LPC_GPIO0->FIOPIN = FIRST_SEG;   
					flag++;
	     }
			 else if(flag==1)
			 {
			 temp2 = SEVEN_CODE[tens]; 
		   LPC_GPIO0->FIOPIN = SECOND_SEG;   
					flag--;
			 }

			 		temp2=temp2<<4;
					LPC_GPIO0->FIOMASK=0xFFFFF00F;
					LPC_GPIO0->FIOPIN=temp2;
					for(j=0; j<1000;j++);
		 }
		else
		{
			
			if(n_flag==0)
			{
			LPC_GPIO0->FIOMASK=0xFFF87FFF;
       if(flag==0)		// For Segment U8
	    {
		   temp2 = SEVEN_CODE[unit]; 
		   LPC_GPIO0->FIOPIN = FIRST_SEG;   
					flag++;
	     }
			 else if(flag==1)
			 {
			 temp2 = SEVEN_CODE[tens]; 
		   LPC_GPIO0->FIOPIN = SECOND_SEG;   
					flag++;
			 }
			 else
			 {
			 temp2 = 0x40; 
		   LPC_GPIO0->FIOPIN = THIRD_SEG;   
		   	flag=0;
				}

			 		temp2=temp2<<4;
					LPC_GPIO0->FIOMASK=0xFFFFF00F;
					LPC_GPIO0->FIOPIN=temp2;
					for(j=0; j<1000;j++);
			
			
			
		}
}
}
}
		
		


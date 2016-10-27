
#include <lpc17xx.h>
#define RS_CTRL  0x08000000  //P0.27
#define EN_CTRL  0x10000000  //P0.28
#define DT_CTRL  0x07800000  //P0.23 to P0.26 data lines
void lcd_init(void);
void wr_cn(void);
void clr_disp(void);
void delay_lcd(unsigned int);
void lcd_com(void);						   
void wr_dn(void);
void lcd_data(void);
void clear_ports(void);
void lcd_puts(unsigned char *);

unsigned int i=0,count=0,n,rem,j=0,k,l,x;
unsigned long int temp1=0, temp2=0 ;
unsigned char Msg1[4],Msg2[4] ;

int main(void)
 {
	 LPC_PINCON->PINSEL4 |= (~(3<<24));
	 LPC_GPIO2->FIODIR &= (~(3<<24));
	 SystemInit();
	 SystemCoreClockUpdate();
  
    lcd_init();				//initialise LCD
	delay_lcd(32000);
	while(1)
	{
			for(x=0;x<j;x++)
			{
				Msg1[x]=' ';
				Msg2[x]=' ';
			}
			if(LPC_GPIO2->FIOPIN&(1<<12))
 			{
				n=i;
				j=0;
				while(n>0)
				{
					rem=n%10;
					Msg1[j] = rem+'0';
					j++;
					n=n/10;
				}
				for(k=j-1,l=0;l<j;k--,l++)
				{
					Msg2[l]=Msg1[k];
				}
				temp1 = 0x80;			//1st message on LCD 1st line
				lcd_com();
				delay_lcd(10000);
				lcd_puts(Msg2);
				i=i+1;
				if(i==1000)
				{
						i=0;
				}
			}
			else if(!(LPC_GPIO2->FIOPIN&(1<<12)))
			{
				n=i;
				j=0;
				while(n>0)
				{
					rem=n%10;
					Msg1[j] = rem+'0';
					j++;
					n=n/10;
				}
				for(k=j-1,l=0;l<j;k--,l++)
				{
					Msg2[l]=Msg1[k];
				}
				temp1 = 0x80;			//1st message on LCD 1st line
				lcd_com();
				delay_lcd(10000);
				lcd_puts(Msg2);
				i=i-1;
				if(i==0)
				{
						i=1000;
				}
			}
  }
 }
 //lcd initialization
 void lcd_init()
 {
	/* Ports initialized as GPIO */
    LPC_PINCON->PINSEL1 &= 0xFC003FFF;  //P0.23 to P0.28

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
	delay_lcd(8000);
    return;
 }

 void lcd_com(void)
 {
	temp2 = temp1 & 0xf0;//move data (26-8+1) times : 26 - HN place, 4 - Bits
	temp2 = temp2 << 19;//data lines from 23 to 26
	wr_cn();
	temp2 = temp1 & 0x0f; //26-4+1
	temp2 = temp2 << 23; 
	wr_cn();
	delay_lcd(10000);
    return;
 }

 // command nibble o/p routine
 void wr_cn(void)                        //write command reg
 { 	 
	clear_ports();
	LPC_GPIO0->FIOPIN = temp2;		// Assign the value to the data lines    
    LPC_GPIO0->FIOCLR = RS_CTRL;		// clear bit RS
	LPC_GPIO0->FIOSET = EN_CTRL;   	// EN=1
	delay_lcd(4500);
	LPC_GPIO0->FIOCLR = EN_CTRL;		 		// EN =0
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
    delay_lcd(10000);	
    return;
 } 

 // data nibble o/p routine
 void wr_dn(void)
 {  	  
	clear_ports();

	LPC_GPIO0->FIOPIN = temp2;		// Assign the value to the data lines    
	LPC_GPIO0->FIOSET = RS_CTRL;	// set bit  RS
	LPC_GPIO0->FIOSET = EN_CTRL;   	// EN=1
	delay_lcd(4500);
	LPC_GPIO0->FIOCLR = EN_CTRL;	// EN =0
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
	LPC_GPIO0->FIOCLR = RS_CTRL;  //Clearing RS line
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
    return;
 }

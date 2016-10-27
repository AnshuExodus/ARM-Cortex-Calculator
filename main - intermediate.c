/* ----------------------------------
* Embedded Systems Project ( CCE )
* ----------------------------------
*
* Group Members:
* 1. Suvodeep Misra - 140953022
* 2. Anitha - 140953023
* 3. Piyush Raisinghani - 140953027
*
* ----------------------------------
*
* Pin Configuration and Settings:
*
* Port 0: Pin 4,5,6,7,8,9,10,11 -> 7-Seg Data Lines ( GPIO OUTPUT )
* Port 1: Pin 23,24,25,26 -> 7-Seg Select Lines ( GPIO OUTPUT )
* Port 0: Pin 15,16,17,18 -> 4x4 Matrix Keypad Rows 1-4 ( GPIO OUTPUT )
* Port 0: Pin 19,20,21,22 -> 4x4 Matrix Keypad Columns 1-4 ( GPIO INPUT )
*
* 10 pin FRC cable connected from CNC to CNB3 ( 4x4 Matrix Keypad - Rows 1-4, Columns 1-4 )
* 10 pin FRC cable connected from CNB to CNB2 ( 7-Seg Select Lines )
* 10 pin FRC cable connected from CNA to CNA2 ( 7-Seg Data Lines )
*/

#include "LPC17xx.h"

#define MAX_NUMBERS 10
#define MAX_DIGITS 3

enum BUTTON
{
    KEY_0, KEY_1, KEY_2, KEY_3,
    KEY_4, KEY_5, KEY_6, KEY_7,
    KEY_8, KEY_9, ACCEPT, CLEAR,
	TMP1, TMP2, TMP3, TMP4
} key[4][4] =
{
    {KEY_0, KEY_1, KEY_2, KEY_3},
    {KEY_4, KEY_5, KEY_6, KEY_7},
    {KEY_8, KEY_9, ACCEPT, CLEAR},
	{TMP1, TMP2, TMP3, TMP4}
};

const uint8_t offset[16] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7c,0x39,0x5e,0x79,0x71};

uint8_t dig[MAX_DIGITS], dig_c = 0;

void Delay(uint32_t i)
{
    while(--i);
}

void ON_KEY_PRESS(uint8_t r/*row*/, uint8_t c/*col*/)
{
    volatile static uint8_t sum_c = 0, total = 0, clr = 0, i;
    volatile static uint16_t sum = 0, mul = 1;
	
    // Execute Scheduled Clear Display
    if(clr)
    {
        dig_c = 0;
        clr = 0;
    }
	
    // Display must be cleared on every Key Press
    if(key[r][c] == ACCEPT) // Digit Input Finish
    {
        // Atleast 1 Digit should be there
        if(!dig_c)
			return;
			
        // Parse Number from Digits and ADD to sum
        mul = 1;
        for(i = 0; i < dig_c; ++i)
        {
            sum += dig[i] * mul;
            mul *= 10;
        }
		
        // Clear Display
        dig_c = 0;
		
        // Set Total Numbers to Input
        if(!total)
        {
            if(sum && sum <= MAX_NUMBERS)
				total = sum; // Set Total Numbers
            else
            {
                dig[0] = 0xF;
                dig_c = 1;
                clr = 1; // Schedule Clear Display on Next Key Press
            }
            sum_c = 0;
            sum = 0;
        }
        else if(++sum_c >= total) // Test for Completion of "total" Input
        {
            while(sum)
            {
                dig[dig_c++] = sum % 10;
                sum /= 10;
            }
            sum_c = 0;
            total = 0;
            clr = 1; // Schedule Clear Display on Next Key Press
        }
    }
    else if(key[r][c] == CLEAR) // Digit Input Discard
    {
        for(i = 0; i < dig_c - 1; ++i)
			dig[i] = dig[i+1];
		dig_c--;
    }
    else if(dig_c < 3) // Digit 0-9 Input ( Maximum 3 )
    {
        for(i = dig_c; i > 0; --i)
			dig[i] = dig[i-1];
        dig[0] = key[r][c];
        dig_c++;
    }
}

void TIMER0_IRQHandler(void)
{
    volatile static uint16_t i, k;
	if(dig_c)
	{
		for(i = 0; i < dig_c; ++i)
		{
			LPC_GPIO1->FIOMASK = (uint32_t) ~(0xF << 23); // Mask Select Lines
			LPC_GPIO1->FIOPIN = (uint32_t) i << 23; // Select Segment
			LPC_GPIO0->FIOMASK = (uint32_t) ~(0xFF << 4); // Mask Data Lines
			LPC_GPIO0->FIOPIN = (uint32_t) offset[dig[i]] << 4; // Set Data
			for(k = 0; k < 20; ++k);	// Delay for Data Set In
			LPC_GPIO0->FIOCLR = 0xFF << 4;	// Clear Data Lines
		}
	}
	else
	{
		LPC_GPIO1->FIOMASK = (uint32_t) ~(0xF << 23); // Mask Select Lines
		LPC_GPIO1->FIOPIN = (uint32_t) 4 << 23; // Disable Segments
	}
	LPC_GPIO1->FIOMASK = 0; // Unmask All
	LPC_GPIO0->FIOMASK = 0; // Unmask All
    LPC_TIM0->IR = 0x01; // Reset Interrupt for MR0
}

void EINT3_IRQHandler(void)
{
	if((LPC_GPIOINT->IO0IntStatR >> 15) & 0xF)
    {
		uint32_t i, col = 0, row = 0;
		
        // Parse Column
        for(i = 0; i <= 3; i++)
        {
            if((LPC_GPIO0->FIOPIN >> 15) & (1 << i))
            {
                if(col)
					goto EINT3_END;
                col = i + 1;
            }
        }
		if(!col)
			goto EINT3_END;
		
        // Parse Row
        LPC_GPIO0->FIOCLR = (uint32_t) 0xF << 19; // Init Pins 0.19 to 0.22 as LOW
        for(i = 0; i <= 3; i++)
        {
            LPC_GPIO0->FIOSET = (uint32_t) 1 << (19 + i);
			Delay(20);
            if((LPC_GPIO0->FIOPIN >> 15) & 0xF)
            {
                if(row)
					goto EINT3_END;
                row = i + 1;
            }
            LPC_GPIO0->FIOCLR = (uint32_t) 1 << (19 + i);
			Delay(20);
        }
		if(!row)
			goto EINT3_END;
		
        // Event KeyPress
        ON_KEY_PRESS(row - 1, col - 1);
		
		EINT3_END:
		Delay(100000);
		LPC_GPIO0->FIOSET = (uint32_t) 0xF << 19; // Set Pins 0.19 to 0.22 as HIGH
    }
    LPC_GPIOINT->IO0IntClr |= (uint32_t) (0xF << 15); // Clear All GPIO Interrupts on Pin 0.15 to 0.18
}

int main(void)
{
    SystemInit();
    SystemCoreClockUpdate();
	
    // Init 7-SEG
    LPC_PINCON->PINSEL0 &= 0xFF0000FF;	// Set Pins 0.4 to 0.11 as GPIO ( 7-SEG DATA LINES )
    LPC_GPIO0->FIODIR |= 0x00000FF0;	// Set Pins 0.4 to 0.11 as OUTPUTS
    LPC_PINCON->PINSEL3 &= 0xFFC03FFF;	// Set Pins 1.23 to 1.26 as GPIO ( 7-SEG SELECT LINES )
    LPC_GPIO1->FIODIR |= 0x07800000;	// Set Pins 1.23 to 1.26 as OUTPUTS
	
    // Init 4x4 Matrix Keypad
    LPC_PINCON->PINSEL0 &= 0x3FFFFFFF;	// Set Pins 0.15 as GPIO ( COLUMN 0 )
	LPC_PINCON->PINSEL1 &= 0xFFFFC000;	// Set Pins 0.16 to 0.18 as GPIO ( COLUMN 1, 2, 3 ) 
										// and Pins 0.19 to 0.22 as GPIO ( ROWS )
    LPC_GPIO0->FIODIR &= 0xFFF87FFF;	// Set Pins 0.15 to 0.18 as INPUTS
    LPC_GPIO0->FIODIR |= 0x00780000;	// Set Pins 0.19 to 0.22 as OUTPUTS
	
    // Set Refresh Timer for 7-SEG
    LPC_SC->PCONP |= 1 << 1;	// Timer0 Power On
    LPC_TIM0->CTCR = 0x00;	// Timer
    LPC_TIM0->TC = 0;	// Initial Count
    LPC_TIM0->PR = 0;	// Prescale
    LPC_TIM0->MR0 = 2999;	// Max Count ( 1 ms )
    LPC_TIM0->MCR = 0x03;	// Match Control (stop = 0, reset = 1, interrupt = 1)
    LPC_TIM0->TCR = 1;	// Start Timer
    NVIC_EnableIRQ(TIMER0_IRQn);	// Enable Timer Interrupt
	
    // Set GPIO External Interrupt
    LPC_GPIOINT->IO0IntEnR |= (uint32_t) 0xF << 15;	// Setup GPIO Rising Edge Interrupt on Pins 0.15 to 0.18
    LPC_GPIO0->FIOSET = (uint32_t) 0xF << 19;	// Set Pins 0.19 to 0.22 as HIGH
    NVIC_EnableIRQ(EINT3_IRQn);	// Enable Global GPIO Interrupt using EINT3
	LPC_GPIOINT->IO0IntClr |= (uint32_t) 0xF << 15;	// Clear All GPIO Interrupts on Pins 0.15 to 0.18
	
    // END
    while(1);
}

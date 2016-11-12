# ARM-Cortex-Calculator
A basic calculator run on an ARM Cortex (A3) Development Board

Software needed:

1. Keil uVision4
2. Flash Magic

Code: Calculator.c

------------



KEIL UVISION 4
--------------


Create a new project in Keil uVision4 by selecting Project-> New uVision Project -> NXP-> LPC1768

Import system files from the Keil directory under ARM for the 'LPC 17xx' module

Check 'Add Hex File' under the Output tab of 'Options for Target'

Add Calculator.c to the Source Group under the Target module

Debug and press F7 to create a Hex file



FLASH MAGIC
-----------


LPC1768

COM Port 1

Baud Rate 9600

Interface None (ISP)

Oscillator 12 (MHz) 

Load Hex File

Start


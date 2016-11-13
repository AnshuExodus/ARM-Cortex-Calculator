# ARM-Cortex-Calculator
A basic calculator run on an ARM Cortex (A3) Development Board

Software needed:

1. Keil uVision4
2. Flash Magic

Code: Calculator.c

------------

ALGORITHM / PSEUDOCODE
----------------------


Algorithm/Pseudocode:

For input and display functionalities

set linePointer to 0
if mode is 0
(has 2 operands and one operator)
scan for first operands
to display convert it to ascii
increment line pointer
scan for operaotrs
to display convert it to ascii
increment line pointer
scan for second operands
to display convert it to ascii
increment line pointer
else 
(has 1 operand and one operator)
scan for operators
to display convert it to ascii
increment line pointer
scan for operand
to display convert it to ascii
increment line pointer

if mode is 0
if operator=='+' add both operands and save result to a
if operator=='-' add both operands and save result to a
if operator=='*' add both operands and save result to a
if operator=='.' add next operand as decimal pointer
if operator=='=' print a in ascii
else
if operator=='log' set a to log(operand)
if operator=='sin' set a to sin(operand)
if operator=='tan' set a to tan(operand)
if operator=='cos' set a to cos(operand)
if operator=='e^' set a to e^(operand)
if operator=='^' set a to ^(operand)
if operator=='powten' set a to 10^(operand)
if operator=='/' set a to /(operand)
if operator=='factorial' set a to factorial(operand)
if operator=='pi' set a to pi(operand)
else print 'goodbye'

For keypad interfacing


Normal Calculator (m) = 0

1	 2	 3	 4

5 	 6 	 7 	 8

9	 0 	 . 	 =

+	 -	 *	 (m) 


Scientific Calculator (m) = 1

/	 ^	 sin 	cos

tan	 pi	 log	 sqr

fact 	 e^	 sqrt	 powten

g o o d b y e 	(m)


polling
col set to input												     
row set to output
loop: make row high
check col input
store row col
loop
send value to array seven_code[row][col]
extract keypress in ascii

if data
fetch higher nibble
then
fetch lower nibble

display lcd

if mode = 0
simple calculator function map
if mode = 1
scientific calculator function map



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


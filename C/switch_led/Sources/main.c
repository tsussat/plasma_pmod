
/*
This code can be used to control the Switchs and the LEDs (RGB and little green LEDs).
File: main.c
Author: Igor Papandinas
Last Update: 17/01/2018
Plasma, coprocessor address:	CTRL_SL_RW		-->	0x400000C0
								CTRL_SL_RST		-->	0x400000C4
*/

#include "../../shared/plasma.h"
#include "../../shared/plasmaCoprocessors.h"
#include "../../shared/plasmaMyPrint.h"

#define MemoryRead(A)     (*(volatile unsigned int*)(A))
#define MemoryWrite(A,V) *(volatile unsigned int*)(A)=(V)


int main(int argc, char ** argv) {

int sw, value;

	MemoryWrite(CTRL_SL_RST, 0); 			// Reset the sw/led controler
	while (1) {
		sw = MemoryRead(CTRL_SL_RW); 		// Read the state of the switches
		value =  (sw<<16) | sw ; 			// MSByte drives the 2 RBG Led (6 bit), LSByte drives the led
		my_printf("value =", value); 		// Display the value on the UART
		MemoryWrite(CTRL_SL_RW, value);
	}
}

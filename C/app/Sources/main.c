/*
This code describes a catalog application for the plasma processor.
It illustrates the functionning of several PMODs using the OledRGB and previou
modules implements on the plasma such as pmodoledrgb_terminal,
pmodoledrgb_bitmap or ctrl_SL. 

File: main.c
Author: Igor Papandinas
Last Update: 25/01/2018
Plasma, coprocessor address:	OLEDTERMINAL_RW		-->	0x400000AC
								OLEDTERMINAL_RST	-->	0x400000A4
								
								CTRL_SL_RST			--> 0x400000C0
								CTRL_SL_RW			--> 0x400000C4
*/


#include "../../shared/plasma.h"
#include "../../shared/plasmaCoprocessors.h"
#include "../../shared/plasmaSoPCDesign.h"
#include "../../shared/plasmaMyPrint.h"

#define MemoryRead(A)     (*(volatile unsigned int*)(A))
#define MemoryWrite(A,V) *(volatile unsigned int*)(A)=(V)


// This function measures the string lenght from the buffer
int bufferLenght(const char* buffer)
{
    int lenght = 0;
    char caracterPointed = 0;

    do
    {
        caracterPointed = buffer[lenght];
        lenght++;
    }
    while(caracterPointed != '\0');

    lenght--; // -1 because of \0

    return lenght;
}

// Sleep Function in us
void sleep(unsigned int us) {
	unsigned int t0 = MemoryRead(TIMER_ADR);
	while(MemoryRead(TIMER_ADR) - t0 < (500 * us))
		;
}





int main( int argc, char ** argv ) {

// Variables and Buffers declaration
	int sw, i = 0;
	
	char bufferPMODGyro[16] =  	"  PMOD GYRO(1)";                  
	char bufferPMODCmps[16] =  	"  PMOD CMPS(16)";

// Reset the oled_rgb PMOD and the sw/led controler
	MemoryRead(OLEDTERMINAL_RST);
	MemoryRead(CTRL_SL_RST);



// Infinite Loop
	while(1) {
	
		while(!MemoryRead(OLEDTERMINAL_RW)) {}							// Screen Clear (Black Background by defaulf)
			MemoryWrite(OLEDTERMINAL_RW, 0x01000000);

		sw = MemoryRead(CTRL_SL_RW); 									// Read the state of the switches


// PMOD Gyro, Switch 1
		if (sw == 0x0001) {
			puts("\rPMOD GYRO selected."); 								// Display the value on the UART
			for (i = 0; i < bufferLenght(bufferPMODGyro); i++) {
    			while(!MemoryRead(OLEDTERMINAL_RW)) {}
	   				MemoryWrite(OLEDTERMINAL_RW, bufferPMODGyro[i]);
	   		}
		   	while (sw == 0x0001)										// Locked until switch 1 is desactivated
		   		sw = MemoryRead(CTRL_SL_RW);
	   	}
	   	
	   	
// PMOD Cmps, Switch 8 	
	   	else if (sw == 0x8000) {
			puts("\rPMOD CMPS selected."); 								// Display the value on the UART
			for (i = 0; i < bufferLenght(bufferPMODCmps); i++) {
    			while(!MemoryRead(OLEDTERMINAL_RW)) {}
	   				MemoryWrite(OLEDTERMINAL_RW, bufferPMODCmps[i]);
	   		}
	   	
		   	while (sw == 0x8000)										// Locked until switch 16 is desactivated
		   		sw = MemoryRead(CTRL_SL_RW);
		}


// No switch activate
	   	else
	   		puts("\rNo switch activate.");
	}
}

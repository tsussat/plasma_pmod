/*
This code can be used to test the TERMINAL module of the OledRGB PMOD develop by
Yannick Bornat.
Entire sentences can be displayed on the screen using this module.

File: main.c
Author: Igor Papandinas
Last Update: 17/01/2018
Plasma, coprocessor address:	OLEDTERMINAL_RW		-->	0x400000A4
								OLEDTERMINAL_RST	-->	0x400000AC
*/

#include "../../shared/plasma.h"
#include "../../shared/plasmaCoprocessors.h"
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


int main( int argc, char ** argv ) {

	int i = 0;
	char buffer[128] =  "                "
                      	"                "
						"                "
	                    " Terminal Module"
	                    "     Working    "
	                    "                "
	                    "                "
	                    "                ";

// Reset the oled_rgb PMOD
	MemoryWrite(OLEDTERMINAL_RST, 1);

// Screen Clear (Black Background by defaulf)
	while(!MemoryRead(OLEDTERMINAL_RW)) {}
		MemoryWrite(OLEDTERMINAL_RW, 0x01000000);


// Buffer Print
  	for (i = 0; i < bufferLenght(buffer); i++) {
    	while(!MemoryRead(OLEDTERMINAL_RW)) {}
	   		MemoryWrite(OLEDTERMINAL_RW, buffer[i]);
  	}

	while(1)
		;
}

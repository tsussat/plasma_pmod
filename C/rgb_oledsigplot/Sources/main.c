/*
This code can be used to test the SIGPLOT module of the OledRGB PMOD develop by
Yannick Bornat.
Entire sentences can be displayed on the screen using this module.

File: main.c
Author: Igor Papandinas
Last Update: 17/01/2018
Plasma, coprocessor address:	OLEDSIGPLOT_RW	-->	0x400000D8
								OLEDSIGPLOT_RST	-->	0x400000D0
*/

#include "../../shared/plasma.h"
#include "../../shared/plasmaCoprocessors.h"
#include "../../shared/plasmaMyPrint.h"

#define MemoryRead(A)     (*(volatile unsigned int*)(A))
#define MemoryWrite(A,V) *(volatile unsigned int*)(A)=(V)



int main( int argc, char ** argv ) {

	MemoryWrite(OLEDSIGPLOT_RST, 1); 			//OledRGB Reset
	puts("Reset done");

	while(!MemoryRead(OLEDSIGPLOT_RW)) {}		//Screen Clear (Black Background by defaulf)
		MemoryWrite(OLEDSIGPLOT_RW, 0x0001111);
	
	puts("sigplot");
	
	while(1)
		;
}

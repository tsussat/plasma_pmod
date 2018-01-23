/*
This code can be used to test the CHARMAP module of the OledRGB PMOD develop by
Yannick Bornat.
Some caracters can be displayed on the screen using this module.

File: main.c
Author: Igor Papandinas
Last Update: 17/01/2018
Plasma, coprocessor address:	OLEDCHARMAP_RW	-->	0x400000A8
								OLEDCHARMAP_RST	-->	0x400000A0
*/

#include "../../shared/plasma.h"
#include "../../shared/plasmaCoprocessors.h"
#include "../../shared/plasmaMyPrint.h"

#define MemoryRead(A)     (*(volatile unsigned int*)(A))
#define MemoryWrite(A,V) *(volatile unsigned int*)(A)=(V)


//Function used to print the caracter 'car' at a given position (row, col)
void printCar(char row, char col, char car) {
	int buff = 0x00000000;

	buff = col;
	buff = (buff << 8) | row;
	buff = (buff << 8) | car;

	while(!MemoryRead(OLEDCHARMAP_RW)) {}			//Wait until ready='1'
		MemoryWrite(OLEDCHARMAP_RW, buff);
}


int main(int argc, char ** argv) {

	MemoryWrite(OLEDCHARMAP_RST, 1); 			//OledRGB Reset

	while(!MemoryRead(OLEDCHARMAP_RW)) {}		//Screen Clear (Black Background by defaulf)
		MemoryWrite(OLEDCHARMAP_RW, 0x01000000);

//Displaying: "Charmap"
	printCar(0x03, 0x01, 0x43);
	printCar(0x03, 0x02, 0x68);
	printCar(0x03, 0x03, 0x61);
	printCar(0x03, 0x04, 0x72);
	printCar(0x03, 0x05, 0x6D);
	printCar(0x03, 0x06, 0x61);
	printCar(0x03, 0x07, 0x70);
	
//Displaying: "Module"
	printCar(0x03, 0x09, 0x4D);
	printCar(0x03, 0x0A, 0x6F);
	printCar(0x03, 0x0B, 0x64);
	printCar(0x03, 0x0C, 0x75);
	printCar(0x03, 0x0D, 0x6C);
	printCar(0x03, 0x0E, 0x65);
	
//Displaying: "Working"
	printCar(0x04, 0x05, 0x57);
	printCar(0x04, 0x06, 0x6F);
	printCar(0x04, 0x07, 0x72);
	printCar(0x04, 0x08, 0x6B);
	printCar(0x04, 0x09, 0x69);
	printCar(0x04, 0x0A, 0x6E);
	printCar(0x04, 0x0B, 0x67);

	while(1)
		;
		
}

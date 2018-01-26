/*
This code can be used to test the NIBBLEMAP module of the OledRGB PMOD develop by
Yannick Bornat.
This module controls the OLEDrgb Pmod to display hexadecimal chars in an array.

File: main.c
Author: Igor Papandinas
Last Update: 18/01/2018
Plasma, coprocessor address:	OLEDTERMINAL_RW		-->	0x400000A4
								OLEDTERMINAL_RST	-->	0x400000AC
*/

#include "../../shared/plasma.h"
#include "../../shared/plasmaCoprocessors.h"
#include "../../shared/plasmaMyPrint.h"

#define MemoryRead(A)     (*(volatile unsigned int*)(A))
#define MemoryWrite(A,V) *(volatile unsigned int*)(A)=(V)

//Function used to print the caracter 'car' at a given position (row, col)
void printCar(char row, char col, char car) {
	int buff = 0x00000000;

	buff = car;
	buff = (buff << 8) | row;
	buff = (buff << 8) | col;

	while(!MemoryRead(OLEDNIBBLEMAP_RW)) {}			//Wait until ready='1'
		MemoryWrite(OLEDNIBBLEMAP_RW, buff);
}


int main(int argc, char ** argv) {

	MemoryWrite(OLEDNIBBLEMAP_RST, 1); 			//OledRGB Reset

	while(!MemoryRead(OLEDNIBBLEMAP_RW)) {}		//Screen Clear (Black Background by defaulf)
		MemoryWrite(OLEDNIBBLEMAP_RW, 0x01000000);

//Displaying: "Charmap"
	printCar(0x03, 0x01, 0x41);
	printCar(0x03, 0x01, 0x42);
	printCar(0x03, 0x02, 0x43);
	printCar(0x03, 0x03, 0x44);
	printCar(0x03, 0x04, 0x45);
	printCar(0x03, 0x05, 0x50);
	printCar(0x03, 0x06, 0x60);
	printCar(0x03, 0x07, 0x70);
	printCar(0x03, 0x09, 0x29);
	printCar(0x03, 0x0A, 0x30);
	printCar(0x03, 0x0B, 0x40);
	printCar(0x03, 0x0C, 0xB0);
	printCar(0x03, 0x0D, 0xC);
	printCar(0x03, 0x0E, 0xD);
	printCar(0x03, 0x0F, 0xE);
/*
//Displaying: "Working"
	printCar(0x04, 0x05, 0x57);
	printCar(0x04, 0x06, 0x6F);
	printCar(0x04, 0x07, 0x72);
	printCar(0x04, 0x08, 0x6B);
	printCar(0x04, 0x09, 0x69);
	printCar(0x04, 0x0A, 0x6E);
	printCar(0x04, 0x0B, 0x67);
*/
	while(1)
		;

}

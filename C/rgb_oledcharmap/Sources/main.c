
#include "../../shared/plasma.h"
#include "../../shared/plasmaCoprocessors.h"
#include "../../shared/plasmaMyPrint.h"

#define MemoryRead(A)     (*(volatile unsigned int*)(A))
#define MemoryWrite(A,V) *(volatile unsigned int*)(A)=(V)



void printCar( char row, char col, char car) {
	int buff = 0x00000000;

	buff = col;
	buff = (buff << 8) | row;
	buff = (buff << 8) | car;

	while( !MemoryRead(OLEDCHARMAP_RW) ) {}
		MemoryWrite(OLEDCHARMAP_RW, buff);
}





int main(int argc, char ** argv) {

	int oled, i;
	char row = 0x00;
	char col = 0x00;
	char car = 0x41;

		MemoryWrite(OLEDCHARMAP_RST, 1); // reset the oled_rgb

		while( !MemoryRead(OLEDCHARMAP_RW) ) {}		//Screen Clear (Black Background by defaulf)
			MemoryWrite(OLEDCHARMAP_RW, 0x01000000);


		for( row = 0; row < 2; row++ ) {
			for( col = 0; col < 16; col++ ) {
				printCar( row, col, car);
				car++;
			}
		}

	while(1) {
	}
}


#include "../../shared/plasma.h"
#include "../../shared/plasmaCoprocessors.h"
#include "../../shared/plasmaMyPrint.h"

#define MemoryRead(A)     (*(volatile unsigned int*)(A))
#define MemoryWrite(A,V) *(volatile unsigned int*)(A)=(V)


//Function used to print the caracter 'car' at a given position (row, col)
void printPixel(char row, char col, int color) {
	int buff = 0x00000000;

	buff = color;
	buff = (buff << 8) | row;
	buff = (buff << 8) | col;

	MemoryWrite(OLEDBITMAP_RW, buff);
}


int main(int argc, char ** argv) {

	MemoryWrite(OLEDBITMAP_RST, 1); // Reset the oled_rgb PMOD

	printPixel(25, 32, 0xF800);
	printPixel(25, 33, 0xF800);
	printPixel(25, 34, 0xF800);
	printPixel(25, 35, 0xF800);
	printPixel(25, 36, 0xF800);

/*
	MemoryWrite(OLEDBITMAP_RW, 0xF8001920);
	MemoryWrite(OLEDBITMAP_RW, 0xF8001921);
	MemoryWrite(OLEDBITMAP_RW, 0xF8001922);
	MemoryWrite(OLEDBITMAP_RW, 0xF8001923);
	MemoryWrite(OLEDBITMAP_RW, 0xF8001924);
*/	
	MemoryWrite(OLEDBITMAP_RW, 0xF8001A20);
	MemoryWrite(OLEDBITMAP_RW, 0xF8001B20);
	MemoryWrite(OLEDBITMAP_RW, 0xF8001C20);
	MemoryWrite(OLEDBITMAP_RW, 0xF8001D20);
	MemoryWrite(OLEDBITMAP_RW, 0xF8001E20);
	MemoryWrite(OLEDBITMAP_RW, 0xF8001F20);
	MemoryWrite(OLEDBITMAP_RW, 0xF8002020);
	
	MemoryWrite(OLEDBITMAP_RW, 0xF8001A24);
	MemoryWrite(OLEDBITMAP_RW, 0xF8001B24);
	MemoryWrite(OLEDBITMAP_RW, 0xF8001C24);
	MemoryWrite(OLEDBITMAP_RW, 0xF8001D24);
	MemoryWrite(OLEDBITMAP_RW, 0xF8001E24);
	MemoryWrite(OLEDBITMAP_RW, 0xF8001F24);
	MemoryWrite(OLEDBITMAP_RW, 0xF8002024);
	
	MemoryWrite(OLEDBITMAP_RW, 0xF8002020);
	MemoryWrite(OLEDBITMAP_RW, 0xF8002021);
	MemoryWrite(OLEDBITMAP_RW, 0xF8002022);
	MemoryWrite(OLEDBITMAP_RW, 0xF8002023);
	
/*
	MemoryWrite( OLEDBITMAP_RW, 0x01002132 );

	MemoryWrite( OLEDBITMAP_RW, 0x00002232 );

	MemoryWrite( OLEDBITMAP_RW, 0x111F2332 );

	MemoryWrite( OLEDBITMAP_RW, 0x111F2432 );

	MemoryWrite( OLEDBITMAP_RW, 0x111F2532 );
*/

	while(1)
		;
}

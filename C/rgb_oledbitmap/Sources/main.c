
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

	'O'
	printPixel(25, 32, 0xF800);
	printPixel(25, 33, 0xF800);
	printPixel(25, 34, 0xF800);
	printPixel(25, 35, 0xF800);
	printPixel(25, 36, 0xF800);
	
	printPixel(31, 32, 0xF800);
	printPixel(31, 33, 0xF800);
	printPixel(31, 34, 0xF800);
	printPixel(31, 35, 0xF800);
	printPixel(31, 36, 0xF800);
	
	printPixel(26, 32, 0xF800);
	printPixel(27, 32, 0xF800);
	printPixel(28, 32, 0xF800);
	printPixel(29, 32, 0xF800);
	printPixel(30, 32, 0xF800);
	
	printPixel(26, 36, 0xF800);
	printPixel(27, 36, 0xF800);
	printPixel(28, 36, 0xF800);
	printPixel(29, 36, 0xF800);
	printPixel(30, 36, 0xF800);
	
	
	'K'
	printPixel(25, 42, 0x07C0);	
	printPixel(26, 42, 0x07C0);
	printPixel(27, 42, 0x07C0);
	printPixel(28, 42, 0x07C0);
	printPixel(29, 42, 0x07C0);
	printPixel(30, 42, 0x07C0);
	printPixel(31, 42, 0x07C0);
	
	printPixel(29, 43, 0x07C0);
	printPixel(28, 44, 0x07C0);
	printPixel(27, 45, 0x07C0);
	printPixel(26, 46, 0x07C0);
	printPixel(25, 47, 0x07C0);
	
	printPixel(29, 45, 0x07C0);
	printPixel(30, 46, 0x07C0);
	printPixel(31, 47, 0x07C0);
	
	
	'!'
	printPixel(25, 53, 0x001F);	
	printPixel(26, 53, 0x001F);
	printPixel(27, 53, 0x001F);
	printPixel(28, 53, 0x001F);
	printPixel(31, 53, 0x001F);
	

	while(1)
		;
}

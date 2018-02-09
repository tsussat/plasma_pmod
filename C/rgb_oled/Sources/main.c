#include "../../shared/plasma.h"
#include "../../shared/plasmaCoprocessors.h"
#include "../../shared/plasmaMyPrint.h"

#define MemoryRead(A)     (*(volatile unsigned int*)(A))
#define MemoryWrite(A,V) *(volatile unsigned int*)(A)=(V)

//Function used to color a pixel at a given position (row, col)
void printPixel(char row, char col, int color)
{
	int buff = 0x00000000;

	buff = color;
	buff = (buff << 8) | row;
	buff = (buff << 8) | col;

	MemoryWrite(OLED_BITMAP_RW, buff);
}

void rgb_oled_bitmap(void)
{
	MemoryWrite(OLED_MUX, OLED_MUX_BITMAP);
	MemoryWrite(OLED_BITMAP_RST, 1); // Reset the oled_rgb PMOD

	// 'O'
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
	
	
	// 'K'
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
	
	
	// '!'
	printPixel(25, 53, 0x001F);	
	printPixel(26, 53, 0x001F);
	printPixel(27, 53, 0x001F);
	printPixel(28, 53, 0x001F);
	printPixel(31, 53, 0x001F);
}


void printCar( char row, char col, char car)
{
	int buff = 0x00000000;

	buff = col;
	buff = (buff << 8) | row;
	buff = (buff << 8) | car;

	while( !MemoryRead(OLED_CHARMAP_RW) ) {}
		MemoryWrite(OLED_CHARMAP_RW, buff);
}

void rgb_oled_charmap(void)
{
	char row = 0x00;
	char col = 0x00;
	char car = 0x41;

	MemoryWrite(OLED_MUX, OLED_MUX_CHARMAP);
	MemoryWrite(OLED_CHARMAP_RST, 1); // reset the oled_rgb

	while( !MemoryRead(OLED_CHARMAP_RW)) {}		//Screen Clear (Black Background by defaulf)
		MemoryWrite(OLED_CHARMAP_RW, 0x01000000);

	for( row = 0; row < 2; row++ ) {
		for( col = 0; col < 16; col++ ) {
			printCar( row, col, car);
			car++;
		}
	}
}

int bufferLength(const char* buffer)
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


void rgb_oled_terminal(void)
{
	char buffer[16] =  "  RGB OLED!";                  
	int i;

	MemoryWrite(OLED_MUX, OLED_MUX_TERMINAL);
	MemoryWrite(OLED_TERMINAL_RST, 1); // reset the oled_rgb

	// Screen Clear (Black Background by defaulf)
	while(!MemoryRead(OLED_TERMINAL_RW)) {}
		MemoryWrite(OLED_TERMINAL_RW, 0x01000000);

	for (i = 0; i < bufferLength(buffer); i++) {
		while(!MemoryRead(OLED_TERMINAL_RW)) {}
	   		MemoryWrite(OLED_TERMINAL_RW, buffer[i]);
	}
}

int main(int argc, char ** argv)
{
	rgb_oled_charmap();
//	rgb_oled_bitmap();
//	rgb_oled_terminal();

	while(1) ;
}

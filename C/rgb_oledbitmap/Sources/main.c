
#include "../../shared/plasma.h"
#include "../../shared/plasmaCoprocessors.h"
#include "../../shared/plasmaMyPrint.h"

#define MemoryRead(A)     (*(volatile unsigned int*)(A))
#define MemoryWrite(A,V) *(volatile unsigned int*)(A)=(V)


//Calculation of x^y
int pow(int x, int y) {

	int result, i =0;
	
	result = x;
	
	for(i = 0; i < y; i++)
		result = result * x;
	
	return result;
}

//Function to convert a decimal number to its hexadecimal expression
int decimal_to_hexadecimal(int x)
{
      int hexadecimal_number, remainder, count = 0;
      for(count = 0; x > 0; count++)
      {
            remainder = x % 16;
            hexadecimal_number = hexadecimal_number + remainder * pow(10,count);
            x = x / 16;
      }
      return hexadecimal_number;
}

//Function used to print the caracter 'car' at a given position (row, col)
void printPixel(int row, int col, int color) {
	int buff = 0x00000000;
	int rowConvert = 0;
	int colConvert = 0;
	
	rowConvert = decimal_to_hexadecimal(row);
	colConvert = decimal_to_hexadecimal(col);

	buff = color;
	buff = (buff << 8) | rowConvert;
	buff = (buff << 8) | colConvert;

	MemoryWrite(OLEDBITMAP_RW, buff);
}


int main( int argc, char ** argv ) {

	MemoryWrite( OLEDBITMAP_RST, 1 ); // Reset the oled_rgb PMOD

	printPixel(25, 32, 0xF800);
	printPixel(25, 33, 0xF800);
	printPixel(25, 33, 0xF800);
	printPixel(25, 33, 0xF800);
	printPixel(25, 33, 0xF800);

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

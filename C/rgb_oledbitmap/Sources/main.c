
#include "../../shared/plasma.h"
#include "../../shared/plasmaCoprocessors.h"
#include "../../shared/plasmaMyPrint.h"

#define MemoryRead(A)     (*(volatile unsigned int*)(A))
#define MemoryWrite(A,V) *(volatile unsigned int*)(A)=(V)


int main( int argc, char ** argv ) {

	MemoryWrite( OLEDBITMAP_RST, 1 ); // Reset the oled_rgb PMOD



	MemoryWrite( OLEDBITMAP_RW, 0x001F2030 );

  MemoryWrite( OLEDBITMAP_RW, 0x001F2531 );

  MemoryWrite( OLEDBITMAP_RW, 0x001F2532 );

  MemoryWrite( OLEDBITMAP_RW, 0x001F2533 );

  MemoryWrite( OLEDBITMAP_RW, 0x001F2534 );


  MemoryWrite( OLEDBITMAP_RW, 0x01002132 );

  MemoryWrite( OLEDBITMAP_RW, 0x00002232 );

  MemoryWrite( OLEDBITMAP_RW, 0x111F2332 );

  MemoryWrite( OLEDBITMAP_RW, 0x111F2432 );

  MemoryWrite( OLEDBITMAP_RW, 0x111F2532 );


	while(1) {
	}
}

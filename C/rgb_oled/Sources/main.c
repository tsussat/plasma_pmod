
#include "../../shared/plasma.h"
#include "../../shared/plasmaCoprocessors.h"
#include "../../shared/plasmaMyPrint.h"

#define MemoryRead(A)     (*(volatile unsigned int*)(A))
#define MemoryWrite(A,V) *(volatile unsigned int*)(A)=(V)


int main(int argc, char ** argv) {

int oled, i;

	MemoryWrite(OLED_RST, 1); // reset the oled_rgb

while(1) {

	//oled = MemoryRead(OLED_READ); // read the state of the oled
	//my_printf("ready =", oled); // if ready=1, the oled is ready to receive a char
	while( !MemoryRead(OLED_RW) ) {}
		MemoryWrite(OLED_RW, 0x00000041);

	//oled = MemoryRead(OLED_READ); // read the state of the oled
	//my_printf("ready =", oled); // if ready=1, the oled is ready to receive a char
	while( !MemoryRead(OLED_RW) ) {}
		MemoryWrite(OLED_RW, 0x00020241);
}
}


#include "../../shared/plasma.h"

#define MemoryRead(A)     (*(volatile unsigned int*)(A))
#define MemoryWrite(A,V) *(volatile unsigned int*)(A)=(V)


int main(int argc, char ** argv) {
	unsigned long buttons;
	puts("Hello world !\n");

	while (1) {
		//if (MemoryRead(0x20000110) == 0)
		//	continue;

		buttons = MemoryRead(0x20000100);
		puts("button status: ");
		print_hex(buttons);
		puts("\n");
	}
}

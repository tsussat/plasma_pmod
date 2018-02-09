#include "../../shared/plasmaSoPCDesign.h"
#include "../../shared/plasma.h"

#define MemoryRead(A)     (*(volatile unsigned int*)(A))
#define MemoryWrite(A,V) *(volatile unsigned int*)(A)=(V)

int main(int argc, char ** argv) {
	unsigned long buttons;

	puts("Test des boutons poussoirs\n");

	while (1) {
		if (MemoryRead(BUTTONS_CHANGE) == 0)
			continue;

		buttons = MemoryRead(BUTTONS_VALUES);
		puts("buttons status: ");
		print_hex(buttons);
		puts("\n");
	}
}

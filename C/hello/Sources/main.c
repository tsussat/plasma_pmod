#include "../../shared/plasmaSoPCDesign.h"
#include "../../shared/plasma.h"

#define MemoryRead(A)     (*(volatile unsigned int*)(A))
#define MemoryWrite(A,V) *(volatile unsigned int*)(A)=(V)


/*int main(int argc, char ** argv) {
	unsigned long buttons;

	int H = 480; // H of the VGA screen in pixel
	int W = 640; // W of the VGA screen in pixel

	puts("Hello world !\n");

	while (1) {
		if (MemoryRead(BUTTONS_CHANGE) == 0)
			continue;

		buttons = MemoryRead(BUTTONS_VALUES);
		puts("button status: ");
		print_hex(buttons);
		puts("\n");

		for(int py = 0; py < H; py++)
		{
			for(int px = 0; px < W; px ++)
			{
				int i = px+py;
				int pixel = ((px>>4)<<8|(py>>4)<<4|(i>>4));
				MemoryWrite(0x20000120, pixel);
			}
		}
	}
}*/

void sleep( unsigned int us )
{
	unsigned int t0 = MemoryRead( TIMER_ADR  );
	while ( MemoryRead( TIMER_ADR  ) - t0 < 500*us )
		;
}

int main(int argc, char ** argv) {
	puts("Test des afficheurs sept segments\n");

	int i;
	for (i = 0; i< 2000; i++)
	{
		MemoryWrite(SEVEN_SEGMENT_BASE, i);
		sleep(5000);
	}

	MemoryWrite(SEVEN_SEGMENT_BASE, 15);
}

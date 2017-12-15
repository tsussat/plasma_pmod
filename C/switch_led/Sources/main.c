
#include "../../shared/plasma.h"
#include "../../shared/plasmaCoprocessors.h"
#include "../../shared/plasmaMyPrint.h"

#define MemoryRead(A)     (*(volatile unsigned int*)(A))
#define MemoryWrite(A,V) *(volatile unsigned int*)(A)=(V)


int main(int argc, char ** argv) {

int sw, value;

	MemoryWrite(CTRL_SL_RST, 0); // reset the sw/led controler
	while (1) {
		sw = MemoryRead(CTRL_SL_READ); // read the state of the switches
		value =  (sw<<16) | sw ; // MSByte drives the 2 RBG Led (6 bit), LSByte drives the led
		my_printf("value =", value); // display the value on the UART
		MemoryWrite(CTRL_SL_WRITE, value);
	}
}

/*--------------------------------------------------------------------
 * TITLE: Plasma DDR Initialization
 * AUTHOR: Steve Rhoads (rhoadss@yahoo.com)
 * DATE CREATED: 12/17/05
 * FILENAME: ddr_init.c
 * PROJECT: Plasma CPU core
 * COPYRIGHT: Software placed into the public domain by the author.
 *    Software 'as is' without warranty.  Author liable for nothing.
 * DESCRIPTION:
 *    Plasma DDR Initialization
 *    Supports 64MB (512Mb) MT46V32M16 by default.
 *    For 32 MB and 128 MB DDR parts change AddressLines and Bank shift:
 *    For 32 MB change 13->12 and 11->10.  MT46V16M16
 *    For 128 MB change 13->14 and 11->12. MT46V64M16
 *--------------------------------------------------------------------*/

#include "../../shared/plasmaCoprocessors.h"
#include "../../shared/plasmaIsaCustom.h"
#include "../../shared/plasmaMisc.h"
#include "../../shared/plasmaSoPCDesign.h"
#include "../../shared/plasmaMyPrint.h"

#define DDR_BASE 0x10000000

extern  int kbhit(void);
extern  int getch(void);
extern  int putchar(int value);
typedef void (*FuncPtr)(void);

extern void ProgramExec();
extern int ReceiveProgram();

int kbhit(void)
{
    return MemoryRead(IRQ_STATUS) & IRQ_UART_READ_AVAILABLE;
}

int getch(void)
{
    while(!kbhit()) ;
    return MemoryRead(UART_READ);
}

unsigned char wait_data()
{
	while( !(MemoryRead(IRQ_STATUS) & IRQ_UART_READ_AVAILABLE) );
	unsigned char cc = MemoryRead(UART_READ);
	return cc;
}

unsigned char wait_data_quiet()
{
	while( !(MemoryRead(IRQ_STATUS) & IRQ_UART_READ_AVAILABLE) );
	unsigned char cc = MemoryRead(UART_READ);
	return cc;
}

void print_err(unsigned long num){
	puts("Boot loader Error, unexpected received data");
	switch(num)
	{
		case 2:
			puts("The 1st received key is different from the expected one, should be 0x30\n");
			break;
		case 3:
			puts("The 2nd received key is different from the expected one, should be 0x31\n");
			break;
		case 4:
			puts("The 3rd received key is different from the expected one, should be 0x32\n");
			break;
		case 5:
			puts("The 4th received key is different from the expected one, should be 0x33\n");
			break;

		default:
			print_err( 2 );
		break;
        }
	 putchar( '0' + (unsigned int)num ); puts("\n");
}

void flush_input_data()
{
	while( kbhit() )
	{
	    getch();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ProgramExec()
{
    FuncPtr funcPtr;
    puts("> BOOTING..."); puts("\n");
	funcPtr = (FuncPtr)0x10000000; // jump to main()
	funcPtr();
	puts("> COMING BACK...\n");
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int ReceiveProgram()
{

	unsigned char *ptr = (unsigned char*)DDR_BASE;
	unsigned char data;

	data = wait_data(); // read the 2nd key, should be : 0x31
	if( data != 0x31 ) // check the key value
	{
        	print_err( 3 );
        	return 0;
	}
	data = wait_data(); // read the 3th key, should be : 0x32
	if( data != 0x32 ) 
	{
        	print_err( 4 );
        	return 0;
	}
    
	data = wait_data(); // read the 4th key, should be : 0x33
	if( data != 0x33 ) 
	{
        	print_err( 5 );
        	return 0;
	}
    
	//
	// RETRIEVE PROGRAM SIZE
	// 

	unsigned char data1 = wait_data(); // READ 4 BYTES THAT SHOULD REPRESENT THE PROGRAM SIZE
	unsigned char data2 = wait_data();
	unsigned char data3 = wait_data();
	unsigned char data4 = wait_data();

	unsigned int sSize = (data4 << 24) | (data3 << 16) | (data2 << 8) | data1;

	// READ THE PROGRAM OF SIZE sSize from address DDR_BASE (0x10000000)
	for(int y=0; y<sSize; y++){
		unsigned char dataS = wait_data();
		ptr[y] = dataS;
	}
	// Go back to DDR_BASE adress
	ptr = (unsigned char*)DDR_BASE;
	return 1;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
	int ch;
	puts("\n\n\n");
	puts("uBoot-Loader (UART) ");
	puts(__DATE__); puts(" "); puts(__TIME__); puts("\n");
	
	flush_input_data();
	puts("(I) Waiting program from UART\n");
	
   	for(;;)
   	{
        while( (kbhit() == 0) && (i_empty() == 1) ) ;
        
 	ch = wait_data();
	switch(ch)
	{
		case 0x30:
			ReceiveProgram();
			ProgramExec();
			break;
		default:
			print_err( 2 );
		break;
        }
    }
    return 0;
}

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
	//putchar(cc);
	return cc;
}

unsigned char wait_data_quiet()
{
	while( !(MemoryRead(IRQ_STATUS) & IRQ_UART_READ_AVAILABLE) );
	unsigned char cc = MemoryRead(UART_READ);
	return cc;
}


void print_err(unsigned long num){
	puts("  + ERR (X) : "); putchar( '0' + (unsigned int)num ); puts("\n");
}

void flush_input_data()
{
	while( kbhit() )
	{
	    getch();
	}
	while( !i_empty() )
	{
	    i_read();
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void ProgramExec()
{
    FuncPtr funcPtr;
    puts("> BOOTING..."); puts("\n");
	funcPtr = (FuncPtr)0x10000000;
	funcPtr();
	puts("> COMING BACK...\n");
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//
// LE PROTOCOLE DE RECEPTION DU PROGRAMME EXECUTABLE EST LE SUIVANT
// [1 x INT] VALEUR DE SYNCRHO (0xFF00EE11)
// [1 x INT] TAILLE DU PROGRAMME (NOMBRE DE INT)
// [1 x INT] VALEUR DE SYNCRHO (0xFF00EE11)
// [n x INT] LES DONNEES DU PROGRAMME
//

int ReceiveProgramFromPCIe()
{
	unsigned int *ptr = (unsigned int*)DDR_BASE;
	unsigned int data;
	unsigned int sSize;
	unsigned int pSum = 0;;
    
	while(i_empty() == 1);
	data = (unsigned int)i_read();
    
	if( data != 0xFF00EE11 )
	{
        print_err( 1 );
        return 0;
	}
    
	while(i_empty() == 1);
	sSize = (unsigned int)i_read(); // ON LIT LA DONNEE 0xFF
    
	while(i_empty() == 1);
	data = (unsigned int)i_read(); // ON LIT LA DONNEE 0xFF
	if( data != 0xFF00EE11 )
	{
        print_err( 2 );
        return 0;
	}
    
	int y = 0;
	while(sSize--)
	{
		while(i_empty() == 1);
		unsigned int value = (unsigned int)i_read();
        pSum    += value;
        ptr[y++] = value;
	}  
//   	puts("  + #  OCTETS : "); print_hex( (unsigned int)y ); puts(" CRC: ");  print_hex( pSum ); puts("\n");
	return 1;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int ReceiveProgram()
{
    //
    // ON RECUPERE LA FIN DE L'EN TETE DU MESSAGE
    //
    unsigned char *ptr = (unsigned char*)DDR_BASE;
    int y;
    
	unsigned char data;
	data = wait_data(); // ON LIT LA DONNEE 0xFF
	//my_printf("ch=",(int)(data));
	if( data != 0x31 )
	{
        print_err( 3 );
        return 0;
	}
	data = wait_data(); // ON LIT LA DONNEE 0xFF
	//my_printf("ch=",(int)(data));
	if( data != 0x32 ) 
	{
        print_err( 4 );
        return 0;
	}
    
	data = wait_data(); // ON LIT LA DONNEE 0xFF
	//my_printf("ch=",(int)(data));
	if( data != 0x33 ) 
	{
        print_err( 5 );
        return 0;
	}
    
    //
    // ON CALCULE LA TAILLE DU PROGRAMME EXECUTABLE QUE L'ON VA RECEVOIR
    //
	unsigned char data1 = wait_data(); // ON LIT LA DONNEE 0xFF
	//my_printf("d1=",(int)(data1));
	unsigned char data2 = wait_data(); // ON LIT LA DONNEE 0xFF
	//my_printf("d2=",(int)(data2));
	unsigned char data3 = wait_data(); // ON LIT LA DONNEE 0xFF
	//my_printf("d3=",(int)(data3));
	unsigned char data4 = wait_data(); // ON LIT LA DONNEE 0xFF
	//my_printf("d4=",(int)(data4));
	unsigned int sSize = (data4 << 24) | (data3 << 16) | (data2 << 8) | data1;
//	my_printf("size=%d\n",sSize);
    //
    // ON RECUPERE LES N DONNEES PREVUES
    //
    for(y=0; y<sSize; y++){
		unsigned char dataS = wait_data();
		//my_printf("dataS=",(int)(dataS));
		ptr[y] = dataS;
//		my_printf("y=",y);
    }
    ptr = (unsigned char*)DDR_BASE;
    /*my_printf("val0 ",ptr[0]);
    my_printf("val1 ",ptr[1]);
    my_printf("val2 ",ptr[2]);
    my_printf("val3 ",ptr[3]);
    my_printf("valm4 ",ptr[sSize-4]);
    my_printf("valm3 ",ptr[sSize-3]);
    my_printf("valm2 ",ptr[sSize-2]);
    my_printf("valm1 ",ptr[sSize-1]);*/
    return 1;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int main()
{
	int ch;
	puts("\n\n\n");
	puts("uBoot-Loader (UART/PCIe)  v1.1");
	puts(__DATE__); puts(" "); puts(__TIME__); puts("\n");
	puts("(C) B. LE GAL 2012 - 2013\n");


//    unsigned int *ptr = (unsigned int*)DDR_BASE;
    
	//
	// ON TESTE LA MEMOIRE RAM INTERNE (64ko)
	//
	
	flush_input_data();
	puts("(I) Waiting program from PCIe or UART\n");
	
   	for(;;)
   	{
        while( (kbhit() == 0) && (i_empty() == 1) ) ;
        
        // MODE PCIe ?
        if( i_empty() == 0 ){
			puts("(I) Starting PCIe reception\n");
            ReceiveProgramFromPCIe();
            ProgramExec();
            continue;
        }
        
        // MODE UART ...
        ch = wait_data();
        //ch = getch();
        //my_printf("ch=",(int)(ch));
        switch(ch)
        {
            case 'A':
				puts("(I) Starting PCIe reception\n");
                ReceiveProgramFromPCIe();
                ProgramExec();
                break;
                
            case 0x30:
		//puts("R"); //(I) Starting UART reception\n");
                ReceiveProgram();
                ProgramExec();
                break;
                
            default:
		        print_err( 9 );
                break;	
        }
    }
    return 0;
}

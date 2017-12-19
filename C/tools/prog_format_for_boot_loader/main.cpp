#include <stdio.h>
#include <stdbool.h>
#include <cstdlib>
#include <cstdint>
#include <cassert>

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <paths.h>
#include <termios.h>
#include <sysexits.h>
#include <sys/param.h>
#include <sys/select.h>
#include <sys/time.h>	
#include <time.h>
#include <cassert>


#define BUF_SIZE 1000000
#define EXIT_SUCCESS 0

int error = 1;
void send_char(unsigned char c, int fileDescriptor, bool wait = false)
{
//	printf(">> Send [%d - 0x%2.2X]\n", (int)c, (int)c);
	int rBytes, wBytes;
	wBytes = write(fileDescriptor, &c, 1);
	//printf("s= %d \n", (int)c);
	//usleep(1000000);
	//assert( wBytes == 1 );

	/*if( wait == false )
		return;*/

	unsigned char buf = 14;
	//rBytes = read (fileDescriptor, &buf, 1);
	//printf("r= %d \n", (int)buf);
	//assert( rBytes == 1 );
	//assert(c == buf);


/*#if 1
	if( c != buf ){
		printf("  s= 0x%2.2X (%d), r= 0x%2.2X (%c) \n", (int)c, (int)c, (int)buf, (int)buf);
		error -= 1;
		if( error == 0 )
			assert(c == buf);
	}
#else
	assert(c == buf);
#endif*/
	usleep(100);
	
	
	/*while(rBytes != 1 || c != buf)
	{
		wBytes = write(fileDescriptor, &c, 1);
		printf("s= %d \n", (int)c);
		unsigned char buf = 14;
		rBytes = read (fileDescriptor, &buf, 1);
		printf("r= %d \n", (int)buf);
		usleep(100);
	}*/
	
	
}

int main(int argc, char ** argv) {

	char *filename = argv[1];
	char *serial = argv[2];
	FILE *infile   = fopen(filename , "rb");
	if(infile == NULL)
	{
		printf("Can't open : %s\n", filename);
		exit (EXIT_SUCCESS);
	}
	// ON CREE LE TABLEAU TEMPORAIRE UTILISE POUR LA LECTURE DU PROGRAMME
	uint8_t *buf = new unsigned char[BUF_SIZE];
	int size   = (int)fread(buf, 1, BUF_SIZE, infile);
	fclose(infile);
    int fileDescriptor;

	if (argc < 3 || filename == NULL || serial == NULL)
		return 1;

	fileDescriptor = open(serial, O_RDWR | O_NOCTTY );
       if(fileDescriptor == -1)
       {
        	printf("Impossible d'ouvrir ttyUSB1 !\n");
            exit( 0 );
        }
      	struct termios t;
		  tcgetattr(fileDescriptor, &t); // recupÃ¨re les attributs
		  cfmakeraw(&t); // Reset les attributs
		  t.c_cflag     = CREAD | CLOCAL;     // turn on READ
		  t.c_cflag    |= CS8;
		  t.c_cc[VMIN]  = 0;
		  t.c_cc[VTIME] = 50;     // 5 sec timeout
		  cfsetispeed(&t, B115200);
		  cfsetospeed(&t, B115200);
		  tcsetattr(fileDescriptor, TCSAFLUSH, &t); // envoie le tout au driver

	unsigned int *prog = (unsigned int *)buf;
	/*for(int k = 1; k< size/4; k++)
	{
		prog[ k ] = __bswap_32 ( prog[ k ] );
	}*/

	unsigned char clef[4] = {0x33, 0x32, 0x31, 0x30};
    int wBytes;
    send_char(clef[3], fileDescriptor);
	usleep( 10000 );
    send_char(clef[2], fileDescriptor);
	usleep( 10000 );
    send_char(clef[1], fileDescriptor);
	usleep( 10000 );
    send_char(clef[0], fileDescriptor);
    //wBytes = write(fileDescriptor, &p_char[0], 1); assert( wBytes == 1 ); usleep( 10000 );
    //wBytes = write(fileDescriptor, &p_char[1], 1); assert( wBytes == 1 ); usleep( 10000 );
    //wBytes = write(fileDescriptor, &p_char[2], 1); assert( wBytes == 1 ); usleep( 10000 );
    //wBytes = write(fileDescriptor, &p_char[3], 1); assert( wBytes == 1 ); usleep( 10000 );
//    assert( wBytes == 4 );
    //sleep( 1 );

	unsigned char *p_size = (unsigned char*)&size;
    send_char(p_size[0], fileDescriptor);
    send_char(p_size[1], fileDescriptor);
    send_char(p_size[2], fileDescriptor);
    send_char(p_size[3], fileDescriptor);

    //wBytes = write(fileDescriptor, &p_char[0], 1); assert( wBytes == 1 ); usleep( 10000 );
    //wBytes = write(fileDescriptor, &p_char[1], 1); assert( wBytes == 1 ); usleep( 10000 );
    //wBytes = write(fileDescriptor, &p_char[2], 1); assert( wBytes == 1 ); usleep( 10000 );
    //wBytes = write(fileDescriptor, &p_char[3], 1); assert( wBytes == 1 ); usleep( 10000 );
    //sleep( 1 );

    /*unsigned int* p_buff = (unsigned int*)buf;
	for(int k=0; k< size/4; k++)
		p_buff[k] = __bswap_32 ( p_buff[k] );*/
	puts("sending program via UART\n");

	for(int k=0; k< size; k++){
	    //wBytes = write(fileDescriptor, &buf[k], 1);
	    //assert( wBytes == 1 );
	    usleep( 10000 );
	    send_char(buf[k], fileDescriptor);
	    //printf("buf[%d]=%d\n\n",k,(int)buf[k]);
	}

    puts("over\n");
    

//    wBytes = write(fileDescriptor, &size, 4);// * _nb_data);
//    assert( wBytes == 4 );

//    wBytes = write(fileDescriptor, buf, size);// * _nb_data);
//    assert( wBytes == size );
/*
	int _nb_data  = 0;
	int rSize = (size-1)/4;
	uint32_t *p = (uint32_t*)buf;
	p++;     // ON SAUTE LE HEADER DE L'UART

	// ON ALLOUE L'ESPACE MEMOIRE NECESSAIRE AU PROGRAMME
	unsigned int *prog = new unsigned int[BUF_SIZE];

	prog[_nb_data++] = size;

	p++;			// ON TRANSMET LA TAILLE DU PROGRAMME
	rSize--;		// ON DECOMPTE UNE DATA
//	prog[_nb_data++] = 0xFF00EE11;

// OPTIMIZATION	int sum  = 0;
	while( rSize-- )
	{
// OPTIMIZATION		int q = ((*p) << 24) & 0xFF000000;
// OPTIMIZATION		q    |= ((*p) <<  8) & 0x00FF00FF;
// OPTIMIZATION		q    |= ((*p) >>  8) & 0x0000FF00;
// OPTIMIZATION		q    |= ((*p) >> 24) & 0x000000FF;
// OPTIMIZATION		prog[_nb_data++] = q;
		//prog[_nb_data++] = __bswap_32 ( *p );
		prog[_nb_data++] = *p;
// OPTIMIZATION		sum += (q);
		p += 1;
	}
	free( buf );

	unsigned char* p_char = (unsigned char*)prog;
	for(int k=0; k< 4*_nb_data; k++){
		if( k%32 == 0 ) printf("\n");
		printf("0x%2.2X ", p_char[k]);
	}
	printf("\n");
*/


}

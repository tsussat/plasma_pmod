
#include "../../shared/plasma.h"
#include "../../shared/plasmaCoprocessors.h"
#include "../../shared/plasmaMyPrint.h"

#define MemoryRead(A)     (*(volatile unsigned int*)(A))
#define MemoryWrite(A,V) *(volatile unsigned int*)(A)=(V)


int longueurChaine(const char* chaine)
{
    int nombreDeCaracteres = 0;
    char caractereActuel = 0;

    do
    {
        caractereActuel = chaine[nombreDeCaracteres];
        nombreDeCaracteres++;
    }
    while(caractereActuel != '\0'); // On boucle tant qu'on n'est pas arrivé à l'\0

    nombreDeCaracteres--; // On retire 1 caractère de long pour ne pas compter le caractère \0

    return nombreDeCaracteres;
}

/*
// printCar function print a specific caracter 'car' in a given row 'row' and column 'col'.
void printCar( char row, char col, char car) {
	int buff = 0x00000000;

	buff = col;
	buff = (buff << 8) | row;
	buff = (buff << 8) | car;


	while( !MemoryRead(OLED_RW) ) {}
		MemoryWrite(OLED_RW, buff);
}

// printOLED function print a sentence of caracters 'buffer'.
void printOLED( char row, char *buffer) {
	int i = 0;
	int col = 0x00;

	for( i = 0; i < longueurChaine(buffer); i++ ) {
		printCar( row, col, buffer[i] );
		col++;
	}

}
*/

int main( int argc, char ** argv ) {

	int i = 0;
	char buffer1[] = "Henri, Paul...  ";
  char buffer2[] = "Ici le plasma...";
  char buffer3[] = "...Pullup";

	MemoryWrite( OLEDTERMINAL_RST, 1 ); // Reset the oled_rgb PMOD

	while( !MemoryRead( OLEDTERMINAL_RW ) ) {}		// Screen Clear (Black Background by defaulf)
		MemoryWrite( OLEDTERMINAL_RW, 0x01000000 );


  for ( i = 0; i < longueurChaine( buffer1 ); i++ ) {
    while( !MemoryRead( OLEDTERMINAL_RW ) ) {}
	   MemoryWrite( OLEDTERMINAL_RW, buffer1[i] );
  }

  for ( i = 0; i < longueurChaine( buffer2 ); i++ ) {
    while( !MemoryRead( OLEDTERMINAL_RW ) ) {}
	   MemoryWrite( OLEDTERMINAL_RW, buffer2[i] );
  }

  for ( i = 0; i < longueurChaine( buffer3 ); i++ ) {
    while( !MemoryRead( OLEDTERMINAL_RW ) ) {}
	   MemoryWrite( OLEDTERMINAL_RW, buffer3[i] );
  }


	while(1) {
	}
}

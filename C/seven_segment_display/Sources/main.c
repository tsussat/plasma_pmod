#include "../../shared/plasmaSoPCDesign.h"
#include "../../shared/plasma.h"

#define MemoryRead(A)     (*(volatile unsigned int*)(A))
#define MemoryWrite(A,V) *(volatile unsigned int*)(A)=(V)


void sleep( unsigned int ms ) // fonction qui impose un delay en millisecondes 
{	// la fréquence d'horloge vaut 25 MHz
	unsigned int t0 = MemoryRead( TIMER_ADR  );
	while ( MemoryRead( TIMER_ADR  ) - t0 < 2500*ms ) // On compte 2500 périodes pour 1 ms
		;
}

int main(int argc, char ** argv) {
	puts("Test des afficheurs sept segments :\n");

	int i, sw;
	for (i = 0; i< 2000; i++)
	{
		MemoryWrite(SEVEN_SEGMENT_BASE, i);
		sleep(100); // 100ms
	}


	puts("Utilisez les switchs pour definir la valeur à écrire sur les afficheurs.");
	while(1) {
		sw = MemoryRead(CTRL_SL_READ); // lecture sur les switch (16 bits de donnée)
		MemoryWrite(SEVEN_SEGMENT_BASE, sw << 16 | sw); // répétition de sw sur les 16 bits de poid fort
	}

}

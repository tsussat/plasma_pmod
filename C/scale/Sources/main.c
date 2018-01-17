#include <stdio.h>
#include <stdbool.h>

#include "../../shared/plasmaCoprocessors.h"
#include "../../shared/plasmaIsaCustom.h"
#include "../../shared/plasmaMisc.h"
#include "../../shared/plasmaSoPCDesign.h"
#include "../../shared/plasmaMyPrint.h"

#include "../Includes/scale.h"
  
int main(int argc, char ** argv) {

	int i, j;
	unsigned int start_c, stop_c;
	unsigned int col_nb, row_nb, pixel_nb;
	unsigned short tmp;

	pixel_nb = 16;
  
	unsigned char data[16] = {12, 15, 20, 210, 124, 16, 54, 156, 134, 21, 7, 15, 46, 97, 84, 161};

	// Affichage des valeurs d'entrees sur la sortie standard
	for(i=0;i<pixel_nb;i++){
		my_printf(" val=> ", data[i]);
	}	
	puts("Affichage des valeurs initiales termine\n");

	// Start timer
	start_c = r_timer();
	
	//scale_no_opt(data, pixel_nb);
	scale_opt1(data, pixel_nb);
	
  	// stop timer
	stop_c = r_timer();
	
	my_printf("nbre de cycles : ", stop_c-start_c);

	puts("### section NO_OPT terminee ###\n");

	// Affichage des valeurs de sortie sur la sortie standard
	for(i=0;i<pixel_nb;i++){
		my_printf(" scaled val=> ", data[i]);
	}

	// arrêt du programme
	puts("Fin du programme\n");
	//stop();
}



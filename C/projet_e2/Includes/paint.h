#define MAX_ROW 63
#define MAX_COL 95

//Function used to color a pixel at a given position (row, col)
void printPixel(char row, char col, int color)
{
	int buff = 0x00000000;

	buff = color;
	buff = (buff << 8) | row;
	buff = (buff << 8) | col;

	MemoryWrite(OLED_BITMAP_RW, buff);
}

void sleep( unsigned int ms ) // fonction qui impose un delay en millisecondes
{	// la fréquence d'horloge vaut 50 MHz
	unsigned int t0 = MemoryRead( TIMER_ADR  );
	while ( MemoryRead( TIMER_ADR  ) - t0 < 50000*ms ) // On compte 50000 périodes pour 1 ms
		;
}

void clearScreen(short tab[MAX_ROW+1][MAX_COL+1]){
	for (char row=0; row <= MAX_ROW; row++){
		for (char col=0; col <= MAX_COL; col++){
			printPixel(row, col, 0x0000);
			tab[(int) row][(int) col] = 0;
		}
	}
}

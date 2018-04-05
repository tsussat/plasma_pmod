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

//RESET DU RGBOLED
void clearScreen(short tab[MAX_ROW+1][MAX_COL+1]){
	for (char row=0; row <= MAX_ROW; row++){
		for (char col=0; col <= MAX_COL; col++){
			printPixel(row, col, 0x0000);
			tab[(int) row][(int) col] = 0;
		}
	}
}

//COULEUR EN FONCTION DES SWITCHS
int get_color(int sw){
	int color=0x0000;
	if (sw & 0x00000001) {
		color+=0x001F;
	}
	if (sw & 0x00000002) {
		color+=0x07E0;
	}
	if (sw & 0x00000004) {
		color+=0xF800;
	}
	return color;
}

//PRINT CONTOUR RECTANGLE
void printRect(char row1, char col1, char row2, char col2, int color, int write, short **tab){
    //1.1
    if(col1<=col2){
        for(int i=0; i<=(col2-col1);i++){
            printPixel(row1,col1+i,color);
            if(write==1){
                tab[(int) row1][(int) col1+i]=color;
            }
        }
    }
        //1.2
    else{
        for(int i=0; i<=(col1-col2);i++){
            printPixel(row1,col2+i,color);
            if(write==1){
                tab[(int) row1][(int) col2+i]=color;
            }
        }
    }

    //2.1
    if(row1<=row2){
        for(int i=0; i<=(row2-row1)-1;i++){
            printPixel(row1+1+i,col1,color);
            if(write==1){
                tab[(int) row1 + 1 + i][(int) col1]=color;
            }
        }
    }
        //2.2
    else{
        for(int i=0; i<=(row1-row2)-1;i++){
            printPixel(row2+i,col1,color);
            if(write==1){
                tab[(int) row2 + i][(int) col1]=color;
            }
        }
    }

    //3.1 et 3.3
    if(row1<=row2){
        for(int i=0; i<=(row2-row1)-1;i++){
            printPixel(row1 + 1 + i, col2, color);
            if (write==1) {
                tab[(int) row1 + 1 + i][(int) col2] = color;
            }
        }
    }
        //3.2 et 3.4
    else{
        for(int i=0; i<=(row1-row2)-1;i++){
            printPixel(row2 + i,col2,color);
            if(write==1){
                tab[(int) row2 + i][(int) col2]=color;
            }
        }
    }
    //4.1 et 4.2
    if(col1<=col2){
        for(int i=0; i<=(col2-col1)-2;i++) {
            printPixel(row2, col1 + 1 + i, color);
            if (write==1) {
                tab[(int) row2][(int) col1 + 1 + i] = color;
            }
        }
    }
        //4.3 et 4.4
    else{
        for(int i=0; i<=(col1-col2)-2;i++) {
            printPixel(row2, col2 + 1 + i, color);
            if (write==1) {
                tab[(int) row2][(int) col2 + 1 + i] = color;
            }
        }
    }
}

//PRINT RECTANGLE PLEIN
void printRectP(char row1, char col1, char row2, char col2, int color, short **tab){   //1
    if(col1<=col2){
        if(row1<=row2){
           for(int i =0; i<(row2-row1); i++){
               for(int j =0; j<(col2-col1); j++){
                   printPixel(row1 + i,col1 + j,color);
                   tab[(int) row1 + i][(int) col1 + j]=color;
               }
           }
        }
        //2
        else{
            for(int i =0; i<(row1-row2); i++){
                for(int j =0; j<(col2-col1); j++){
                    printPixel(row2 + i,col1 + j,color);
                    tab[(int) row2 + i][(int) col1 + j]=color;
                }
            }
        }
    }
    //3
    else{
        if(row1<=row2){
            for(int i =0; i<(row2-row1); i++){
                for(int j =0; j<(col1-col2); j++){
                    printPixel(row1 + i,col2 + j,color);
                    tab[(int) row1 + i][(int) col2 + j]=color;
                }
            }
        }
        //4
        else{
            for(int i =0; i<(row1-row2); i++){
                for(int j =0; j<(col1-col2); j++){
                    printPixel(row2 + i,col2 + j,color);
                    tab[(int) row2 + i][(int) col2 + j]=color;
                }
            }
        }
    }
}

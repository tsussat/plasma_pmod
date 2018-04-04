#include "../../shared/plasma.h"
#include "../../shared/plasmaCoprocessors.h"
#include "../../shared/plasmaMyPrint.h"
#include "../../shared/plasmaSoPCDesign.h"
#include "../Includes/paint.h"

short tab[64][96] = {0, 1};

#define MemoryRead(A)     (*(volatile unsigned int*)(A))
#define MemoryWrite(A,V) *(volatile unsigned int*)(A)=(V)

int main(int argc, char ** argv){

  int sw, value, color;
  char col = 0;
  char row = 0;
  int val_button;
  int button_change;
  int write = 0; //ecriture sur l'ecran
  int rect = 0; //rectangle actif
  char start[2] = {0, 0}; //coordonées origine rectangle


  MemoryWrite(OLED_MUX, OLED_MUX_BITMAP);
  MemoryWrite(OLED_BITMAP_RST, 1); // Reset the oled_rgb PMOD
  MemoryWrite(CTRL_SL_RST, 1); // reset the sw/led controler

  /*
  //Test
  printPixel(0, 0, 0x001F);
  printPixel(1, 0, 0x07E0);
  printPixel(2, 0, 0xF800);
  int a = readPixel(0, 0);
  int b = readPixel(1, 0);
*/

  clearScreen(tab); //met l'ecran en noir

  while (1) {

    //BOUTONS
    val_button = MemoryRead(BUTTONS_VALUES);
    button_change = MemoryRead(BUTTONS_CHANGE);
    if ((val_button & 0x00000001) && (button_change & 0x00000001)){
      //central button
      write = !(write);
    }
    if (val_button & 0x00000002){
      //up button
      if (row > 0) {
        row--;
      }
    }
    if (val_button & 0x00000004){
      //down button
      if (row < MAX_ROW) {
        row++;
      }
    }
    if (val_button & 0x00000008){
      //left button
      if (col > 0) {
        col--;
      }
    }
    if (val_button & 0x00000010){
      //down button
      if (col < MAX_COL) {
        col++;
      }
    }

    //LED
    sw = MemoryRead(CTRL_SL_RW); // read the state of the switches
    value =  (sw<<16) & 0x00070000 ; // MSByte drives the 2 RBG Led (6 bit), LSByte drives the led
    value = value | (write<<20);
    MemoryWrite(CTRL_SL_RW, value); // drive the LEDs with value

    //RESET
    if(sw & 0x00008000){
      clearScreen(tab);
      while(sw & 0x00008000);
    }
    //RECTANGLE
        //INIT
        else if((sw & 0x00002000) && (rect==0)){
            rect=1;
            start[0] = row;
            start[1] = col;
        }
        //PREVIEW
        else if((sw & 0x00002000) && (rect==1)){
            sleep(50);
            printRect(start[0], start[1], row, col, ~tab[(int) row][(int) col], 0, tab);
            sleep(50);
            printRect(start[0], start[1], row, col, tab[(int) row][(int) col], 0, tab);
        }
        //TRACE
        else if(!(sw & 0x00002000) && (rect==1)){
            rect=0;
            color=get_color(sw);
            if(sw & 0x00001000){
                printRectP(start[0], start[1], row, col, color, tab);
            }
            else {
                printRect(start[0], start[1], row, col, color, 1, tab);
            }
        }
    //FONCTIONNEMENT NORMAL
    else{
      //COULEUR
      color=get_color(sw);
      //ECRITURE
      if (write == 1){
        printPixel(row,col,color);
        tab[(int) row][(int) col]=color;
      }
      //CLIGNOTEMENT
      sleep(50);
      printPixel(row, col , ~tab[(int) row][(int) col]);
      sleep(50);
      printPixel(row, col , tab[(int) row][(int) col]);
    }

  }

}

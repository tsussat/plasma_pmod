#include "../../shared/plasma.h"
#include "../../shared/plasmaCoprocessors.h"
#include "../../shared/plasmaMyPrint.h"
#include "../../shared/plasmaSoPCDesign.h"
#include "../Includes/paint.h"

#define MemoryRead(A)     (*(volatile unsigned int*)(A))
#define MemoryWrite(A,V) *(volatile unsigned int*)(A)=(V)

#define MAX_ROW 63
#define MAX_COL 95

int main(int argc, char ** argv)
{

  int sw, value, color;
  char col = 0;
  char row = 0;

  MemoryWrite(OLED_MUX, OLED_MUX_BITMAP);
  MemoryWrite(OLED_BITMAP_RST, 1); // Reset the oled_rgb PMOD
  MemoryWrite(CTRL_SL_RST, 1); // reset the sw/led controler

  while (1) {
    sw = MemoryRead(CTRL_SL_RW); // read the state of the switches
    value =  (sw<<16) & 0x00070000 ; // MSByte drives the 2 RBG Led (6 bit), LSByte drives the led
    my_printf("value = ", value); // display the value on the UART
    MemoryWrite(CTRL_SL_RW, value); // drive the LEDs with value

    color=0x0000;
    if (sw & 0x00000001) {
      color+=0x001F;
    }
    if (sw & 0x00000002) {
      color+=0x07E0;
    }
    if (sw & 0x00000004) {
      color+=0xF800;
    }
    printPixel(row,col,color);
    if (col<MAX_COL){
      col++;
    }
    else{
      col=0;
      if (row<MAX_ROW){
        row++;
      }
      else{
        row=0;
      }
    }
    sleep(100); // wait 100 ms
  }
}

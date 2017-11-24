#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "../../shared/plasmaCoprocessors.h"
#include "../../shared/plasmaIsaCustom.h"
#include "../../shared/plasmaMisc.h"
#include "../../shared/plasmaSoPCDesign.h"
#include "../../shared/plasmaMyPrint.h"
//#include "../../shared/plasmaFifoInOut.h"

//#define COLOR

#define Nf 18
#define Nr 12

int Convergence(int x_C, int y_C, int Imax)
{
  long long int x = 0;
  long long int y = 0;
  int x2 = (x*x) >> Nf;
  int y2 = (y*y) >> Nf;
  int iter = 0;
  int x_new;
  int quatre = 4<<Nf;
  int mod2 = x2 + y2;
  
  /*while( (mod2 <= quatre) && ( iter < Imax) )
  {

    x_new = isa_custom_2(x, y) + x_C;

    y = isa_custom_1(x,y) + y_C;

    x = x_new;

    mod2 = isa_custom_3(x,y);
    iter++;
  }*/

  while( (mod2 <= quatre) && ( iter < Imax) )
  {
    x_new = x2 - y2 + x_C;
    y = 2*((x*y)>>Nf) + y_C;
    x = x_new;
    x2 = (x*x)>>Nf;
    y2 = (y*y)>>Nf;
    mod2 = x2 + y2;
    iter++;
  }


  return iter;
}

int main( int argc, char ** argv ) {



  //coproc_reset(COPROC_4_RST);
  //coproc_reset(COPROC_1_RST)
  puts("Mandelbrot\n");

  int H = 480; // H of the image in pixel
  int W = 640; // W of the image in pixel
  int Imax = 255;
  int FS = 255; // RGB full scale

  // my_printf("Debut de Mandelbrot :",H);

  int x_C;
  int y_C;

  int x_A = ~(3<<Nf-2)+1; // -1.75 au format A(31-Nf,Nf)
  int y_A = ~(3<<Nf-2)+1; // -1.5 au format A(31-Nf,Nf)
  int x_B = (3<<Nf-2); // 0.75 au format A(31-Nf,Nf)
  int y_B = (3<<Nf-2); // 1.5 au format A(31-Nf,Nf)
  
  long long int dx = (x_B - x_A); // A(Ni+1,Nf)
  long long int dy = (y_B - y_A); // U(Ni+1,Nf)
  int dx64 = dx;
  int dy64 = dy;

  dx64 = ((dx64<<Nr) / W) >> Nr; // A(Ni+1,Nf+12)
  dy64 = ((dy64<<Nr) / H) >> Nr; // A(Ni+1,Nf+12)
  dx = dx64;
  dy = dy64;
	
  //my_printf("dx :", dx);
  //my_printf("dy :", dy);

  for(int py = 0; py < H; py++)
  //for(int py = 400; py < 401; py++)
  {
    int prod = (dy*py);  
    //my_printf("prod", prod);
    y_C = y_A + dy*py;
	//my_printf("py :",py);
    for(int px = 0; px < W; px ++){
    //for(int px = 6; px < 7; px ++){
      //my_printf("px :",px);
      prod = dx*px;
      x_C = x_A + dx*px;
      // my_printf("x_C :", x_C);
      // my_printf("y_C :", y_C);
      //coproc_write(COPROC_1_RW, x_C);
      //coproc_write(COPROC_1_RW, y_C);
      
      int i;
      
      i = Convergence(x_C, y_C, Imax);
      //my_printf("i soft:", i);
      /*i = 1;
      while(i > 0) // tant que le bit de poind fort vaut 0 <=> done == 0
      {
        i = coproc_read(COPROC_1_RW);
      }

      i = i&0x00000FFF;  // iter number is on 12 bits
*/
      // my_printf("i hard:", i);
      // my_printf("i :", i);
      int pixel = ((i>>4)<<8|(i>>4)<<4|(i>>4));
      // my_printf("px :",pixel);
      //coproc_write(COPROC_4_RW, pixel);
      MemoryWrite(0x20000120, pixel);
      //coproc_write(COPROC_4_RW, i);
      //coproc_write(COPROC_4_RW, i);
    }
  }
}




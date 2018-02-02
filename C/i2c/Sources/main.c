// I2C module -- main.c
// Author : Henri
// Description : Example program handling I2C communication with the PMOD compass

#include "../Includes/i2c.h"

// PMOD Compass
#define SLAVE_ADDRESS_CMPS    0x1E
#define CMPS_MODE_REGISTER    2
#define CMPS_STATUS_REGISTER  9
#define CMPS_DATA_X_MSB       3
#define CMPS_DATA_X_LSB       4
#define CMPS_DATA_Y_MSB       7
#define CMPS_DATA_Y_LSB       8


void sleep(unsigned int us)
{
  unsigned int t0 = MemoryRead(TIMER_ADR );
  while (MemoryRead(TIMER_ADR ) - t0 < 500*us)
    ;
}

int main(int argc, char const *argv[]) {

  unsigned int data;
  unsigned int buf[6];

  short x, y, z;

  puts("I2C Module Start\n");

  puts("Compass sensor, continuous mode\n");

  select_mode(0); // 0 for PMOD compass sensor

  start();
  adress_send(SLAVE_ADDRESS_CMPS, WRITE);
  send_data(0x00); // CRA register pointer
  send_data(0x70); // set values
  stop();

  start();
  adress_send(SLAVE_ADDRESS_CMPS, WRITE);
  send_data(0x01); // CRB register pointer
  send_data(0xA0); // set values
  stop();

  start();
  adress_send(SLAVE_ADDRESS_CMPS, WRITE);
  send_data(0x02); // Mode register pointer
  send_data(0x00); // set values
  stop();

  sleep(10000); // 10ms

  while(1) {

    start();
    adress_send(SLAVE_ADDRESS_CMPS, READ);
    receive_data(&buf, 6); // Six readings
    stop();

    x = (short) (buf[0] << 8) + buf[1];
    y = (short) (buf[2] << 8) + buf[3];
    z = (short) (buf[4] << 8) + buf[5];

    start();
    adress_send(SLAVE_ADDRESS_CMPS, WRITE);
    send_data(0x03); // First data register pointer
    stop();   

    puts("x: ");
    print_int((int) x);

    puts("y: ");
    print_int((int) y);

    puts("z: ");
    print_int((int) z);

    sleep(100000); 

  }




  return 0;
}

// I2C module -- main.c
// Author : Henri
// Description : Management of i2c protocol between PLASMA and Pmod devices

#include "../../shared/plasmaSoPCDesign.h"
#include "../../shared/plasma.h"

#define MemoryRead(A)     (*(volatile unsigned int*)(A))
#define MemoryWrite(A,V) *(volatile unsigned int*)(A)=(V)

#define I2C_ADDR      0x40000300  // adress shift left by one, RW first bit
#define I2C_STATUS    0x40000304
#define I2C_CONTROL   0x40000308
#define I2C_DATA      0x4000030c

#define I2C_ADDR_RW             (1 << 0)

#define I2C_STATUS_BUSY         (1 << 0)
#define I2C_STATUS_ACK          (1 << 1)
//#define I2C_STATUS_DATA_READY   (1 << 2)

#define I2C_CONTROL_START       (1 << 0)
#define I2C_CONTROL_STOP        (1 << 1)
#define I2C_CONTROL_DATA_READ   (1 << 2)
#define I2C_CONTROL_DATA_WRITE  (1 << 3)
#define I2C_CONTROL_ACK         (1 << 4)
#define I2C_CONTROL_SELECT      (1 << 5)


#define READ      1
#define WRITE     0

// SLAVE SENSORS REGISTERS ADDRESSES

// TMP3
#define SLAVE_ADDR_TMP3   0x48
#define AMBIENT_TEMP      0

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

void start() {
  ctrl = 0 | I2C_CONTROL_START;
  MemoryWrite(I2C_CONTROL, ctrl); 
}


void stop() {
  ctrl = 0 | (I2C_CONTROL_STOP);
  MemoryWrite(I2C_CONTROL, ctrl); 
}

void wait_busy() {
  unsigned long status;
  while (MemoryRead (I2C_STATUS) & I2C_STATUS_BUSY != 0) {
    status = MemoryRead(I2C_STATUS);
    puts("i2c status: ");
    print_hex(status);
    puts("\n");

    status = MemoryRead(I2C_CONTROL);
    puts("i2c control: ");
    print_hex(status);
    puts("\n");
  }
}


int adress_send(unsigned int addr, unsigned int r_w) {
  
  MemoryWrite(I2C_ADDR, (addr << 1) | r_w;); // address

  wait_busy();

  if (MemoryRead(I2C_STATUS) & I2C_STATUS_ACK != 0)
    return (-1);

  return 0;
}

/*
    Sending data
*/

int send_data(unsigned int data) {

  unsigned int ctrl = 0;

  wait_busy();

  if (MemoryRead(I2C_STATUS) & I2C_STATUS_ACK != 0)
    return (-1);

  ctrl = 0 | I2C_CONTROL_DATA_WRITE;

  MemoryWrite (I2C_DATA, data); // data
  MemoryWrite (I2C_CONTROL, ctrl); // writing start

  wait_busy();

  if (MemoryRead(I2C_STATUS) & I2C_STATUS_ACK != 0) // check ACK
    return (-1);

  return 0;

}


/*
    Receiving data
*/

int receive_data(unsigned int *buf, unsigned int buf_len) {

  unsigned int ctrl;

  wait_busy();

  for (i = 0; i < buf_len; i++) {

    ctrl = 0 | I2C_CONTROL_DATA_READ;

    if (i == buf_len - 1)
      ctrl |= (I2C_CONTROL_NACK); // last element nack = 1
    else
      ctrl &= ~(I2C_CONTROL_NACK); // else nack = 0

    MemoryWrite(I2C_CONTROL, ctrl);

    wait_busy();

    buf[i] = MemoryRead(I2C_DATA);

  }

  return 0;

}



int main(int argc, char const *argv[]) {

  unsigned int data;
  unsigned int buf[6];

  short x, y, z;

  puts("I2C Module Start\n");

  puts("Compass sensor, continuous mode\n");

  MemoryWrite(I2C_CONTROL, 0 | I2C_CONTROL_SELECT); // select the cmps sensor

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

    sleep(100000); // 100ms

  }




  return 0;
}

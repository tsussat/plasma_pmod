// I2C module -- i2c.c
// Author : Henri
// Description : Management of i2c protocol between PLASMA and Pmod devices

#include "../Includes/i2c.h"


void select_mode(unsigned int mode) {
  MemoryWrite(I2C_CONTROL, mode | I2C_CONTROL_SELECT);
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

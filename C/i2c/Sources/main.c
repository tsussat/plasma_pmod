// I2C module -- main.c
// Author : Henri
// Description : Management of i2c protocol between PLASMA and Pmod devices

#include "../../shared/plasma.h"

#define MemoryRead(A)     (*(volatile unsigned int*)(A))
#define MemoryWrite(A,V) *(volatile unsigned int*)(A)=(V)

#define I2C_STATUS    0x40000300
#define I2C_CONTROL   0x40000304
#define I2C_ADDR      0x40000308 // adress shift left by one, RW first bit
#define I2C_DATA      0x40000309

#define I2C_ADDR_RW             (1 << 0)

#define I2C_STATUS_BUSY         (1 << 0)
#define I2C_STATUS_ACK          (1 << 1)
#define I2C_STATUS_DATA_READY   (1 << 2)

#define I2C_CONTROL_START       (1 << 0)
#define I2C_CONTROL_DATA_START  (1 << 1)
#define I2C_CONTROL_STOP        (1 << 2)

#define STOP      0
#define RESTART   0

#define SLAVE_ADDR_TMP3   0x48
#define AMBIENT_TEMP      0

void wait_busy() {
  while (MemoryRead (I2C_STATUS) & I2C_STATUS_BUSY != 0)
    ;
}

/*
    Maitre en mode écriture
*/

int master_write(unsigned int addr, unsigned int data, unsigned int mode) {

  unsigned int ctrl = 0;
  unsigned int addr_rw = 0;

  // selecting slave with address, and put /W to 0
  addr_rw = (addr << 1);
  ctrl |= I2C_CONTROL_START;

  MemoryWrite(I2C_ADDR, addr_rw);
  MemoryWrite(I2C_CONTROL, ctrl);

  wait_busy();

  if (MemoryRead (I2C_STATUS) & I2C_STATUS_ACK != 0)
    return (-1);

  MemoryWrite (I2C_DATA, data);
  MemoryWrite (I2C_CONTROL, ctrl);

  wait_busy();

  if (MemoryRead (I2C_STATUS) & I2C_STATUS_ACK != 0)
    return (-1);

  // PAUSE until RESTART or STOP
  if (mode == STOP) {
    ctrl |= (I2C_CONTROL_STOP);
    MemoryWrite (I2C_CONTROL, ctrl);
    return 0;
  }
  else if (mode == RESTART)
    return 0;
  else
    return (-1);

}


/*
    Maitre en mode lecture
*/

int master_read(unsigned int addr, unsigned int *buf, unsigned int mode) {

  unsigned int ctrl = 0;
  unsigned int addr_rw = 0;
  unsigned int read = 0;

  addr_rw = (addr << 1) | I2C_ADDR_RW;
  ctrl |= I2C_CONTROL_START; // RESTART ?

  MemoryWrite(I2C_ADDR, addr);
  MemoryWrite(I2C_CONTROL, ctrl);

  wait_busy();

  if (MemoryRead(I2C_STATUS) & I2C_STATUS_ACK != 0)
    return (-1);

  read = 1;

  while (read) {

    wait_busy();

    while (MemoryRead (I2C_STATUS) & I2C_STATUS_DATA_READY != 0)
      ;

    *buf = MemoryRead (I2C_DATA);

    if (MemoryRead (I2C_STATUS) & I2C_STATUS_ACK == 1)
      read = 0;
  }

  // PAUSE until RESTART or STOP
  if (mode == STOP) {
    ctrl |= (I2C_CONTROL_STOP);
    MemoryWrite (I2C_CONTROL, ctrl);
    return 0;
  }
  else if (mode == RESTART)
    return 0;
  else
    return (-1);

}



int main(int argc, char const *argv[]) {

  unsigned int data, addr;
  unsigned int buf;

  puts("I2C Module Start\n");

  puts("Get the ambient temperature on PMOD sensor TMP3 (TCN75A)\n");
  master_write (SLAVE_ADDR_TMP3, AMBIENT_TEMP, STOP);
  master_read (SLAVE_ADDR_TMP3, &buf, STOP);

  return 0;
}

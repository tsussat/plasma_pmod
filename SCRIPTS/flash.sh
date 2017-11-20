#!/bin/sh

CFG="nexys4-ddr.cfg"
BITSTREAM="top_plasma.bit"
BINARY="test.bin"

sudo openocd -f "$CFG"  -c "init; jtagspi_init 0 $BITSTREAM;"
sudo ./programmer "$BINARY"

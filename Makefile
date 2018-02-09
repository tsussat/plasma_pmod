# Tools

CC ?= gcc
C++ ?= g++
CC_MIPS ?= mips-elf-gcc
AS_MIPS ?= mips-elf-as
LD_MIPS ?= mips-elf-ld

# Compiler

CFLAGS_MIPS = -O3 -Wall -c -s -funroll-loops -mips1 -mtune=mips1 -msoft-float -std=c99 -DVHDL_SIMULATION
ENTRY = 0x10000000

# Directories

TOP = .

C = $(TOP)/C
HDL = $(TOP)/HDL
BUILD = $(TOP)/BUILD
SIMU = $(TOP)/SIMU
SYNTHESIS = $(TOP)/SYNTHESIS

PLASMA = $(HDL)/PLASMA
TOOLS = $(C)/tools
OBJ = $(TOP)/OBJ
BIN = $(TOP)/BIN

BUILD_DIRS =
BUILD_BINS =

# Tools

CONVERT_BIN = $(BIN)/convert_bin
CONVERT_BIN_SOURCES = $(TOOLS)/convert.c
BUILD_BINS += $(BIN)/convert_bin

PROGRAMMER = $(BIN)/programmer
PROGRAMMER_SOURCES = $(TOOLS)/prog_format_for_boot_loader/main.cpp
BUILD_BINS += $(BIN)/programmer

# C

PROJECTS =

SHARED_FILES = no_os.c
SHARED_FILES_ASM = uboot.asm
SHARED_SOURCES = $(addprefix $(C)/shared/,$(SHARED_FILES))
SHARED_SOURCES_ASM = $(addprefix $(C)/shared/,$(SHARED_FILES_ASM))
SHARED_OBJECTS = $(addprefix $(OBJ)/shared/,$(SHARED_FILES:.c=.o))
SHARED_OBJECTS_ASM = $(addprefix $(OBJ)/shared/,$(SHARED_FILES_ASM:.asm=.o))
BUILD_DIRS += $(OBJ)/shared

BOOT_LOADER = $(OBJ)/boot_loader/code_bin.txt
BOOT_LOADER_FILES = main.c
BOOT_LOADER_SOURCES = $(addprefix $(C)/boot_loader/Sources/,$(BOOT_LOADER_FILES))
BOOT_LOADER_OBJECTS = $(addprefix $(OBJ)/boot_loader/,$(BOOT_LOADER_FILES:.c=.o))
BUILD_DIRS += $(OBJ)/boot_loader

HELLO = $(BIN)/hello.bin
HELLO_FILES = main.c
HELLO_SOURCES = $(addprefix $(C)/hello/Sources/,$(HELLO_FILES))
HELLO_OBJECTS = $(addprefix $(OBJ)/hello/,$(HELLO_FILES:.c=.o))
BUILD_DIRS += $(OBJ)/hello
BUILD_BINS += $(BIN)/hello.bin
PROJECTS += $(HELLO)

MANDELBROT = $(BIN)/mandelbrot.bin
MANDELBROT_FILES = main.c
MANDELBROT_SOURCES = $(addprefix $(C)/mandelbrot/Sources/,$(MANDELBROT_FILES))
MANDELBROT_OBJECTS = $(addprefix $(OBJ)/mandelbrot/,$(MANDELBROT_FILES:.c=.o))
BUILD_DIRS += $(OBJ)/mandelbrot
BUILD_BINS += $(BIN)/mandelbrot.bin
PROJECTS += $(MANDELBROT)

RGB_OLED = $(BIN)/rgb_oled.bin
RGB_OLED_FILES = main.c
RGB_OLED_SOURCES = $(addprefix $(C)/rgb_oled/Sources/,$(RGB_OLED_FILES))
RGB_OLED_OBJECTS = $(addprefix $(OBJ)/rgb_oled/,$(RGB_OLED_FILES:.c=.o))
BUILD_DIRS += $(OBJ)/rgb_oled
BUILD_BINS += $(BIN)/rgb_oled.bin
PROJECTS += $(RGB_OLED)

SWITCH_LED = $(BIN)/switch_led.bin
SWITCH_LED_FILES = main.c
SWITCH_LED_SOURCES = $(addprefix $(C)/switch_led/Sources/,$(SWITCH_LED_FILES))
SWITCH_LED_OBJECTS = $(addprefix $(OBJ)/switch_led/,$(SWITCH_LED_FILES:.c=.o))
BUILD_DIRS += $(OBJ)/switch_led
BUILD_BINS += $(BIN)/switch_led.bin
PROJECTS += $(SWITCH_LED)

I2C = $(BIN)/i2c.bin
I2C_FILES = main.c
I2C_SOURCES = $(addprefix $(C)/i2c/Sources/,$(I2C_FILES))
I2C_OBJECTS = $(addprefix $(OBJ)/i2c/,$(I2C_FILES:.c=.o))
BUILD_DIRS += $(OBJ)/i2c
BUILD_BINS += $(BIN)/i2c.bin
PROJECTS += $(I2C)

# HDL

PLASMA_SOC = $(BIN)/plasma.bit
PLASMA_SOC_FLOW = $(OBJ)/plasma/plasma.tcl
PLASMA_SOC_FILES = alu.vhd \
	bus_mux.vhd \
	buttons.vhd \
	cam_pkg.vhd \
	comb_alu_1.vhd \
	control.vhd \
	conversion.vhd \
	ctrl_7seg.vhd \
	ctrl_SL.vhd \
	dma_engine.vhd \
	mem_ctrl.vhd \
	memory_64k.vhd \
	mlite_cpu.vhd \
	mlite_pack.vhd \
	mux_7seg.vhd \
	mult.vhd \
	mod_7seg.vhd \
	pc_next.vhd \
	pipeline.vhd \
	plasma.vhd \
	pmodoledrgb_bitmap.vhd \
	pmodoledrgb_charmap.vhd \
	pmodoledrgb_nibblemap.vhd \
	pmodoledrgb_sigplot.vhd \
	pmodoledrgb_terminal.vhd \
	pmodoledrgb_charmap.vhd \
	ram_boot.vhd \
	reg_bank.vhd \
	sequ_alu_1.vhd \
	shifter.vhd \
	top_plasma.vhd \
	trans_hexto7seg.vhd \
	txt_util.vhd \
	vga_bitmap_160x100.vhd \
	vga_ctrl.vhd \
	vgd_bitmap_640x480.vhd
PLASMA_SOC_SOURCES = $(addprefix $(PLASMA)/,$(PLASMA_SOC_FILES))

PLASMA_SIMULATION_FILES = tbench.vhd
PLASMA_SIMULATION_SOURCES = $(addprefix $(PLASMA)/,$(PLASMA_SIMULATION_FILES))

PLASMA_SOC_TOP = top_plasma
BUILD_DIRS += $(OBJ)/plasma

SIMU_TEST_BENCH = $(SIMU)/test_bench.prj

# Configuration

CONFIG_PROJECT ?= hello
CONFIG_TARGET ?= nexys4_DDR
CONFIG_PART ?= xc7a100tcsg324-1
CONFIG_SERIAL ?= /dev/ttyUSB1
CONFIG_UART ?= yes
CONFIG_I2C ?= yes

ifeq ($(CONFIG_PROJECT),hello)
PROJECT = $(HELLO)
else ifeq ($(CONFIG_PROJECT),mandelbrot)
PROJECT = $(MANDELBROT)
else ifeq ($(CONFIG_PROJECT),rgb_oled)
PROJECT = $(RGB_OLED)
else ifeq ($(CONFIG_PROJECT),switch_led)
PROJECT = $(SWITCH_LED)
else ifeq ($(CONFIG_PROJECT),i2c)
PROJECT = $(I2C)
endif

PLASMA_SOC_GENERICS =

ifeq ($(CONFIG_UART),yes)
PLASMA_SOC_GENERICS += eUart=1'b1
PLASMA_SOC_FILES += uart.vhd
else
PLASMA_SOC_GENERICS += eUart=1'b0
endif

ifeq ($(CONFIG_I2C),yes)
PLASMA_SOC_GENERICS += eI2C=1'b1
PLASMA_SOC_FILES += i2c.vhd
else
PLASMA_SOC_GENERICS += eI2C=1'b0
endif

PLASMA_SOC_ARGUMENTS = $(foreach generic,$(PLASMA_SOC_GENERICS),-generic $(generic))

TARGET_XDC = $(SYNTHESIS)/$(CONFIG_TARGET).xdc

# Rules

all: plasma project simulation

$(BUILD_DIRS):
	@mkdir -p $@

$(CONVERT_BIN): $(CONVERT_BIN_SOURCES) | $(BUILD_DIRS)
	$(CC) -o $@ $<

.PHONY: convert_bin
convert_bin: $(CONVERT_BIN)

$(PROGRAMMER): $(PROGRAMMER_SOURCES) | $(BUILD_DIRS)
	$(C++) -std=c++11 -o $@ $<

.PHONY: programmer
programmer: $(PROGRAMMER)

.PHONY: send
send: $(PROGRAMMER) $(PROJECT)
	$(PROGRAMMER) $(PROJECT) $(CONFIG_SERIAL)

$(SHARED_OBJECTS): $(OBJ)/shared/%.o: $(C)/shared/%.c | $(BUILD_DIRS)
	$(CC_MIPS) $(CFLAGS_MIPS) -o $@ $<

$(SHARED_OBJECTS_ASM): $(OBJ)/shared/%.o: $(C)/shared/%.asm | $(BUILD_DIRS)
	$(AS_MIPS) -o $@ $<

$(BOOT_LOADER_OBJECTS): $(OBJ)/boot_loader/%.o: $(C)/boot_loader/Sources/%.c | $(BUILD_DIRS)
	$(CC_MIPS) $(CFLAGS_MIPS) -o $@ $<

$(BOOT_LOADER): $(SHARED_OBJECTS_ASM) $(SHARED_OBJECTS) $(BOOT_LOADER_OBJECTS) $(CONVERT_BIN) | $(BUILD_DIRS)
	$(LD_MIPS) -Ttext 0 -eentry -Map $(OBJ)/boot_loader/boot_loader.map -s -N -o $(OBJ)/boot_loader/boot_loader.axf $(SHARED_OBJECTS_ASM) $(SHARED_OBJECTS) $(BOOT_LOADER_OBJECTS)
	$(CONVERT_BIN) $(OBJ)/boot_loader/boot_loader.axf $(OBJ)/boot_loader/boot_loader.bin $@

.PHONY: boot_loader
boot_loader: $(BOOT_LOADER)

$(HELLO_OBJECTS): $(OBJ)/hello/%.o: $(C)/hello/Sources/%.c | $(BUILD_DIRS)
	$(CC_MIPS) $(CFLAGS_MIPS) -o $@ $<

$(HELLO): $(SHARED_OBJECTS_ASM) $(SHARED_OBJECTS) $(HELLO_OBJECTS) $(CONVERT_BIN) | $(BUILD_DIRS)
	$(LD_MIPS) -Ttext $(ENTRY) -eentry -Map $(OBJ)/hello/hello.map -s -N -o $(OBJ)/hello/hello.axf $(SHARED_OBJECTS_ASM) $(SHARED_OBJECTS) $(HELLO_OBJECTS)
	$(CONVERT_BIN) $(OBJ)/hello/hello.axf $@ $(OBJ)/hello/code_bin.txt

.PHONY: hello
hello: $(HELLO)

$(MANDELBROT_OBJECTS): $(OBJ)/mandelbrot/%.o: $(C)/mandelbrot/Sources/%.c | $(BUILD_DIRS)
	$(CC_MIPS) $(CFLAGS_MIPS) -o $@ $<

$(MANDELBROT): $(SHARED_OBJECTS_ASM) $(SHARED_OBJECTS) $(MANDELBROT_OBJECTS) $(CONVERT_BIN) | $(BUILD_DIRS)
	$(LD_MIPS) -Ttext $(ENTRY) -eentry -Map $(OBJ)/mandelbrot/mandelbrot.map -s -N -o $(OBJ)/mandelbrot/mandelbrot.axf $(SHARED_OBJECTS_ASM) $(SHARED_OBJECTS) $(MANDELBROT_OBJECTS)
	$(CONVERT_BIN) $(OBJ)/mandelbrot/mandelbrot.axf $@ $(OBJ)/mandelbrot/code_bin.txt

.PHONY: mandelbrot
mandelbrot: $(MANDELBROT)

$(RGB_OLED_OBJECTS): $(OBJ)/rgb_oled/%.o: $(C)/rgb_oled/Sources/%.c | $(BUILD_DIRS)
	$(CC_MIPS) $(CFLAGS_MIPS) -o $@ $<

$(RGB_OLED): $(SHARED_OBJECTS_ASM) $(SHARED_OBJECTS) $(RGB_OLED_OBJECTS) $(CONVERT_BIN) | $(BUILD_DIRS)
	$(LD_MIPS) -Ttext $(ENTRY) -eentry -Map $(OBJ)/rgb_oled/rgb_oled.map -s -N -o $(OBJ)/rgb_oled/rgb_oled.axf $(SHARED_OBJECTS_ASM) $(SHARED_OBJECTS) $(RGB_OLED_OBJECTS)
	$(CONVERT_BIN) $(OBJ)/rgb_oled/rgb_oled.axf $(RGB_OLED) $(OBJ)/rgb_oled/code_bin.txt

.PHONY: rgb_oled
rgb_oled: $(RGB_OLED)

$(SWITCH_LED_OBJECTS): $(OBJ)/switch_led/%.o: $(C)/switch_led/Sources/%.c | $(BUILD_DIRS)
	$(CC_MIPS) $(CFLAGS_MIPS) -o $@ $<

$(SWITCH_LED): $(SHARED_OBJECTS_ASM) $(SHARED_OBJECTS) $(SWITCH_LED_OBJECTS) $(CONVERT_BIN) | $(BUILD_DIRS)
	$(LD_MIPS) -Ttext $(ENTRY) -eentry -Map $(OBJ)/switch_led/switch_led.map -s -N -o $(OBJ)/switch_led/switch_led.axf $(SHARED_OBJECTS_ASM) $(SHARED_OBJECTS) $(SWITCH_LED_OBJECTS)
	$(CONVERT_BIN) $(OBJ)/switch_led/switch_led.axf $(SWITCH_LED) $(OBJ)/switch_led/code_bin.txt

.PHONY: switch_led
switch_led: $(SWITCH_LED)

$(I2C_OBJECTS): $(OBJ)/i2c/%.o: $(C)/i2c/Sources/%.c | $(BUILD_DIRS)
	$(CC_MIPS) $(CFLAGS_MIPS) -o $@ $<

$(I2C): $(SHARED_OBJECTS_ASM) $(SHARED_OBJECTS) $(I2C_OBJECTS) $(CONVERT_BIN) | $(BUILD_DIRS)
	$(LD_MIPS) -Ttext $(ENTRY) -eentry -Map $(OBJ)/i2c/i2c.map -s -N -o $(OBJ)/i2c/i2c.axf $(SHARED_OBJECTS_ASM) $(SHARED_OBJECTS) $(I2C_OBJECTS)
	$(CONVERT_BIN) $(OBJ)/i2c/i2c.axf $@ $(OBJ)/i2c/code_bin.txt

.PHONY: i2c
i2c: $(I2C)

.PHONY: project
project: $(PROJECT)

.PHONY: projects
projects: $(PROJECTS)

$(PLASMA_SOC): $(PLASMA_SOC_SOURCES) $(BOOT_LOADER) | $(BUILD_DIRS)
	cp $(BOOT_LOADER) $(PLASMA)
	echo "" > $(PLASMA_SOC_FLOW)
	echo "set outputDir $(OBJ)/plasma" >> $(PLASMA_SOC_FLOW)
	for file in $(PLASMA_SOC_SOURCES); do echo "read_vhdl $$file" >> $(PLASMA_SOC_FLOW); done
	echo "read_xdc $(TARGET_XDC)" >> $(PLASMA_SOC_FLOW)
	echo "synth_design -top $(PLASMA_SOC_TOP) -part $(CONFIG_PART) $(PLASMA_SOC_ARGUMENTS)" >> $(PLASMA_SOC_FLOW)
	echo "opt_design" >> $(PLASMA_SOC_FLOW)
	echo "place_design" >> $(PLASMA_SOC_FLOW)
	echo "phys_opt_design" >> $(PLASMA_SOC_FLOW)
	echo "route_design" >> $(PLASMA_SOC_FLOW)
	echo "write_bitstream -force $@" >> $(PLASMA_SOC_FLOW)
	echo "quit" >> $(PLASMA_SOC_FLOW)
	vivado -source $(PLASMA_SOC_FLOW) -mode tcl -nolog -nojournal
	rm $(PLASMA_SOC_FLOW)

.PHONY: plasma
plasma: $(PLASMA_SOC)

.PHONY: bitstream
bitstream: plasma

.PHONY: vhdl
vhdl: plasma

.PHONY: simulation
simulation:
	xsim tbench --nolog -t $(SIMU)/simu_run.tcl

.PHONY: simu
simu: simulation

simulation_modelsim:
	vsim tbench -c -quiet -do "set NumericStdNoWarnings 1; set StdArithNoWarnings 1; run -all; exit"

.PHONY: simu_modelsim
simu_modelsim: simulation_modelsim

.PHONY: clean
clean:
	rm -rf $(BUILD_DIRS)
	rm -f $(BUILD_BINS)

.PHONY: distclean
distclean: clean
	rm -rf $(OBJ)
	rm -rf $(BIN)

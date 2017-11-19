# Makefile permettant de :
# - compiler le code source à exécuter sur le Plasma
# - compiler les fichiers VHDL du SoC Plasma
# - lancer la simulation VHDL du SoC Plasma
# - dé-assembler le code compilé
# Usage:
# - make soft : compile l'exécutable avec le compilateur MIPS (modifié) et génère le fichier .txt correspondant
# - make vhdl : compile les fichiers VHDL du SoC Plasma
# - make simu : lance la simulation VHDL du SoC Plasma
# - make all : fait toutes les étapes précédentes une par une
# - make fpga : make soft + make vhdl + copie le fichier binaire dans le repertoire de synthese
# - make simu_debug : make soft + make vhdl + lancement de la simu modelsim en mode graphique

#MY_PROJECT=ray_tracer_v3
#MY_PROJECT=filtre
#MY_PROJECT=filtre_no_fifo
#MY_PROJECT=tuto_plasma
#MY_PROJECT=mandelbrot
MY_PROJECT=boot_loader
#MY_PROJECT=hello

SRC_DIR=Sources
INC_DIR=Includes
SHARED_DIR=shared
C_DIR=./C
VHDL_DIR=./HDL
SIM_DIR=./SIMU
SYN_DIR=./SYNTHESIS

VHDL_PLASMA_DIR = $(VHDL_DIR)/PLASMA
VHDL_CUSTOM_DIR = $(VHDL_DIR)/custom/$(MY_PROJECT)

SHARED_FILES = \
	$(C_DIR)/$(SHARED_DIR)/plasmaSoPCDesign.h\
	$(C_DIR)/$(SHARED_DIR)/plasmaCoprocessors.h\
	$(C_DIR)/$(SHARED_DIR)/plasmaIsaCustom.h\
	$(C_DIR)/$(SHARED_DIR)/plasmaMisc.h\
	$(C_DIR)/$(SHARED_DIR)/plasmaMyPrint.h

SIM_TOP = tbench
SYN_TOP = top_plasma
OBJ_DIR = OBJ

all: soft vhdl simu

debug: soft vhdl simu_debug

fpga:
	# software compilation
	make soft
	vivado -source $(SYN_DIR)/impl_flow.tcl -mode tcl -nolog -nojournal



CFLAGS      = -O3 -Wall -c -s -funroll-loops -mips1 -mtune=mips1 -msoft-float -std=c99
GCC_MIPS    = mips-elf-gcc $(CFLAGS)
AS_MIPS     = mips-elf-as
LD_MIPS     = mips-elf-ld
CONVERT_BIN = $(C_DIR)/tools/convert_bin.exe

soft:
	$(AS_MIPS) $(C_DIR)/$(SHARED_DIR)/boot.asm -o $(OBJ_DIR)/boot.o
	$(AS_MIPS) $(C_DIR)/$(SHARED_DIR)/uboot.asm -o $(OBJ_DIR)/uboot.o
	$(GCC_MIPS) $(C_DIR)/$(SHARED_DIR)/no_os.c -o $(OBJ_DIR)/no_os.o
	$(GCC_MIPS) $(SHARED_FILES)
	$(GCC_MIPS) -DVHDL_SIMULATION $(C_DIR)/$(MY_PROJECT)/$(INC_DIR)/*.h
	$(GCC_MIPS) -DVHDL_SIMULATION $(C_DIR)/$(MY_PROJECT)/$(SRC_DIR)/*.c
	$(GCC_MIPS) -DVHDL_SIMULATION $(C_DIR)/$(MY_PROJECT)/$(SRC_DIR)/main.c -o $(OBJ_DIR)/main.o
	mv *.o $(OBJ_DIR)
	$(LD_MIPS) -Ttext 0 -eentry -Map $(OBJ_DIR)/test.map -s -N -o test.axf $(OBJ_DIR)/boot.o $(OBJ_DIR)/main.o $(OBJ_DIR)/no_os.o
	$(CONVERT_BIN)
	mv test.axf $(OBJ_DIR)
	cp code_bin.txt $(VHDL_PLASMA_DIR)
	rm $(C_DIR)/$(SHARED_DIR)/*.gch
	rm $(C_DIR)/$(MY_PROJECT)/$(INC_DIR)/*.gch


clean:
	rm -f *.pb
	rm -f output.txt
	rm -f code.txt
	rm -f code_bin.txt
	rm -f *.wdb *.jou
	rm -f *.bin *.map *.axf
	rm -rf xsim.dir
	rm -rf *.log *.jou
	rm -rf *.o *.cf $(OBJ_DIR)/* tb *.vcd main $(SIM_TOP) *.ghw
	rm -f vivado*
	rm -f usage*
	rm -f *.os

vhdl:
	ls HDL/PLASMA/*.vhd > tmp.txt
	sed 's/^/vhdl work ..\//' tmp.txt > $(SIM_DIR)/$(SIM_TOP).prj
	rm tmp.txt
	cp $(VHDL_CUSTOM_DIR)/*.vhd $(VHDL_PLASMA_DIR)
	xelab -prj $(SIM_DIR)/$(SIM_TOP).prj -debug typical $(SIM_TOP) -s $(SIM_TOP) --nolog

simu:
	xsim tbench --nolog -t $(SIM_DIR)/simu_run.tcl
	#rm ./code_bin.txt

send:
	$(GCC_MIPS) -DVHDL_SIMULATION $(C_DIR)/$(MY_PROJECT)/$(SRC_DIR)/main.c
	$(GCC_MIPS) $(C_DIR)/$(SHARED_DIR)/no_os.c -o no_os.o
	$(AS_MIPS) -o uboot.o $(C_DIR)/$(SHARED_DIR)/uboot.asm

	$(LD_MIPS) -Ttext 0x10000000 -eentry -Map test.map -s -N -o test.axf uboot.o main.o no_os.o

	$(CONVERT_BIN)

	$(C_DIR)/tools/prog_format_for_boot_loader/a.out test.bin

dasm:
	mips-elf-gcc -O3 -Wall -c -s -funroll-loops -mips1 -mtune=mips1 -msoft-float -std=c99 -DVHDL_SIMULATION $(C_DIR)/$(MY_PROJECT)/$(SRC_DIR)/main.c -S
	vi main.s

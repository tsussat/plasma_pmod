
set outputDir ./SYNTHESIS
file mkdir $outputDir

read_vhdl [ glob ./HDL/PLASMA/*.vhd ]
#read_xdc ./SYNTHESIS/nexys4.xdc
read_xdc ./SYNTHESIS/nexys4_DDR.xdc
synth_design -top top_plasma -part xc7a100tcsg324-1

opt_design
place_design
phys_opt_design

route_design

write_bitstream -force $outputDir/top_plasma.bit

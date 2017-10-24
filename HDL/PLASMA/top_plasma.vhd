-- Top-level design for ipbus demo
--
-- This version is for xc6vlx240t on Xilinx ML605 eval board
-- Uses the v6 hard EMAC core with GMII interface to an external Gb PHY
--
-- You must edit this file to set the IP and MAC addresses
--
-- Dave Newbold, May 2011

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
--use work.ipbus.ALL;
--use work.bus_arb_decl.all;
--use work.mac_arbiter_decl.all;

library unisim;
use unisim.VComponents.all;      

entity top_plasma is port(       
	clk100: in std_logic;
	rst: in std_logic;
	sw : in std_logic;
	--led: out std_logic_vector(7 downto 0);  
	led: out std_logic;
   i_uart : in std_logic;
   o_uart : out std_logic;
	VGA_hs       : out std_logic;   -- horisontal vga syncr.
   VGA_vs       : out std_logic;   -- vertical vga syncr.
   VGA_red      : out std_logic_vector(3 downto 0);   -- red output
   VGA_green    : out std_logic_vector(3 downto 0);   -- green output
   VGA_blue     : out std_logic_vector(3 downto 0)   -- blue output
	);
end top_plasma;

architecture rtl of top_plasma is
		signal clk50, clk100_sig: std_logic;
		
		--component clk_wiz_0 is -- vivado
--		component clkgen is --ise
--        port
--         (-- Clock in ports
--          clk_in1           : in     std_logic;
--          -- Clock out ports
--          clk_out1          : out    std_logic;
--			 clk_out2          : out    std_logic
--         );
--        end component; 

begin

process(rst, clk50)
begin
	if(rst='1') then
			--o_uart <= '0';
			led <= '0';
	elsif(clk50'event and clk50='1') then
			--o_uart <= i_uart;
			led <= i_uart or sw;
	end if;
end process;

--	DCM clock generation for internal bus, ethernet
--clock_gen : clk_wiz_0 -- vivado
--clock_gen : clkgen -- ise
--  port map
--   (-- Clock in ports
--    CLK_IN1 => clk100,
--    -- Clock out ports
--    CLK_OUT1 => clk50,
--	 CLK_OUT2 => clk100_sig);

		
clk_div : process(clk100, rst)
begin
	if(rst='1') then
		clk50 <= '0';
	elsif(clk100'event and clk100 = '1') then
		clk50 <= not(clk50);
	end if;
end process;
		
		
--	leds(7 downto 0) <= ('0','0','0','0','0','0', locked, onehz);


	Inst_plasma: entity work.plasma
	GENERIC MAP (
		memory_type => "XILINX_16X",
		log_file    => "UNUSED",
      ethernet    => '0',
		eUart       => '1',
      use_cache   => '0'
	)
	PORT MAP(
		clk           => clk50,  
		clk_VGA 		=> clk100,
		reset         => rst,
		uart_write    => o_uart,
		uart_read     => i_uart,
		fifo_1_out_data  => x"00000000",
		fifo_1_read_en   => open,
		fifo_1_empty     => '1',
		fifo_2_in_data   => open,
		fifo_1_write_en  => open,
		fifo_2_full      => '0',

		fifo_1_full      => '0',
		fifo_1_valid     => '0',
		fifo_2_empty     => '1',
		fifo_2_valid     => '0',
		fifo_1_compteur  => x"00000000",
		fifo_2_compteur  => x"00000000",

		VGA_hs => VGA_hs,
		VGA_vs => VGA_vs,
		VGA_red => VGA_red,
		VGA_green => VGA_green,
		VGA_blue => VGA_blue,
		
		gpio0_out       => open,
		gpioA_in        => x"00000000" --open
	);
	
--led <= "01101001";


end rtl;


----------------------------------------------------------------------------------
-- I2C_master
-- V1.2 (2015/12/24 - Yannick Bornat)
-- 
----------------------------------------------------------------------------------
-- This module handles the I2C bus as a master for the hardware part only, this
-- means that knowledge about the behavior of I2C communication is mandatory.
-- 
-- All operations must be performed while the 'ready' bit is set (='1'). Operations
-- require to set (='1') the corresponding input during one clock cycle. All other
-- operation inputs must be cleared (='0').
-- while bus is free, only the 'send' and 'repeat_ack' inputs are sampled, if bus
-- is controlled by the module, all operation inputs are sampled, when bus is
-- controlled by another master (occupied), the 'ready' bit is cleared and no operation
-- input is sampled.
--
-- when the bus is idle or free, setting the 'send' bit will actually send the 8-bit data
-- present in 'data_in'. This data may be actual data, or a device address
-- associated with the R/W bit. If the bus was not controlled by the module, it
-- first performs a 'START' command. During command, the 'ready' bit is cleared (='0').
-- After the command completion, 'data_out' outputs the acknowledge bit (0x00 for ACK /
-- 0xFF for NAK) with 'data_out_en' as enable, and the 'ready' bit is set back to '1'.
-- If the command could not be completed, the 'cancelled' bit is set during one
-- clock period, this may happen if :
--    * the slave issued a STOP instead of ACK or NAK : then the bus is released and
--      the 'ready' bit is set. The master can start a new transfer. This behavior is
--      I2C compliant but is implemented for bus reliability.
--    * another master is speaking on the bus with higher priority. If so, the module
--      goes in a busy mode and the 'ready' bit is set as soon as the bus is available
--      again.
-- 
-- When the bus is idle, setting the 'read_with_ack' or 'read_with_nack' bits for one
-- clock cycle will trigger the bus clock to retreive a data byte. Considering the
-- command, the master module will respond ACK or NAK to the slave. Once completed,
-- received data is output through the 'data_out' signal, controlled by the 'data'out'en'
-- signal. The 'ready' bit finally goes back to '1'.
-- The 'Cancelled' output may be asserted if the slave performs a STOP during data
-- reception, this behavior is not I2C compliant but is implemented for bus reliability
--
-- when the bus is idle, setting the 'repeat_start' bit will actually perform another
-- START sequence without performing a STOP. This is usefull to read a command response
-- from the slave without releasing the bus. Refer to I2C communication for more details.
-- If the bus was free, this command performs a simple START sequence.
--
-- when the bus is idle, setting the 'stop' bit sends a STOP sequence. Once completed
-- the bus is free again and may be used by any master for any operation.
--
----------------------------------------------------------------------------------
-- WARNING :
-- This module only handles the hardware part of the I2C bus and does not perform any
-- check about data consistency. For example, it is possible to start then read the bus
-- but this behavior is not I2C complient and no data will be sent on the SDA line,
-- other devices will then consider that slave 0x7F is addressed for writing.
-- It is also possible to continue writing after sending an address for read (LSB = 1).
-- In such a case, data collision may occur.
----------------------------------------------------------------------------------
--
-- HISTORY
-- V1.1 (update by YB - 2015/12/21)
--   * state machine was rewritten to send data immedately after a falling edge of scl
--     substates are now identified by the position of scli.
-- V1.2 (update by YB - 2015/12/24)
--   * test on time counter is synchronized (less LUTs and higer freq, costs 1 FF)
--
-- IMPROVEMENTS :
--   * merge ack_next_read with main FSM
--
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;


entity I2C_master is
    generic( max_cnt : integer := 499);
    Port ( clk   : in     STD_LOGIC;
           reset : in     STD_LOGIC;
           sda   : inout  STD_LOGIC;
           scl   : inout  STD_LOGIC;
           
           -- bus actions
           data_in        : in  STD_LOGIC_VECTOR (7 downto 0);
           send           : in  STD_LOGIC;  -- send word presented on data_in and eventually issue a START (if needed)
           read_with_ack  : in  STD_LOGIC;  -- read data on the bus and send ACK
           read_with_nack : in  STD_LOGIC;  -- read data on the bus and send NACK
           repeat_start   : in  STD_LOGIC;  -- send a START (R-START if bus is already grabbed)
           stop           : in  STD_LOGIC;  -- send a STOP (release the bus)
           
           -- bus feedback
           ready          : out  STD_LOGIC;  -- module accepts instructions (bus idle or bus grabbed and instruction finished)
           cancelled      : out  STD_LOGIC;  -- slave interrupted transfer or other master speaking (the 'ready' output discriminates)
           data_out       : out  STD_LOGIC_VECTOR (7 downto 0);
           data_out_en    : out  STD_LOGIC);  -- enable bit for data_out. 
                                              -- after a send, data_out = 0 means ACK, data_out = 0xFF means NACK
                                              -- after a read, data_out is the value sent by the slave
end I2C_master;

architecture Behavioral of I2C_master is

--type t_state_phase is (one, two, three, four);
type t_state is (bus_free, pre_restart, mk_start, start_send, send_byte, read_ack, read_byte, send_ack, mk_stop, bus_idle, bus_busy, bus_exc);
   -- bus_free    : nobody talks on the bus, it is free to use
   -- pre_restart : put the bus in proper conditions for a restart
   -- mk_start    : performs a START sequence, then go to bus_waiting
   -- start_send  : performs a START sequence, then go to send_byte
   -- send _byte  : writes a byte on the bus
   -- read_ack    : reads the ACK bit after a send (still controlling the clock)
   -- read_byte   : reads a byte on the bus while controlling the clock
   -- send_ack    : sends an ACK bit to the bus
   -- mk_stop     : performs a STOP sequence on the bus
   -- bus_idle    : bus is controlled by the module which waits for commands to perform
   -- bus_busy    : bus is controlled by someone else
   -- bus_exc     : exception : non I2C compliant activity has been observed on the bus
   --                   ( scl is low without a start sequence )

signal state          : t_state;
--signal state_phase    : t_state_phase;
signal remaining_bits : integer range 0 to 7;         -- bit counter for state machine
signal time_cnt       : integer range 0 to max_cnt;   -- used to produce clk half periods
signal time_over      : std_logic;

signal old_sda        : std_logic;
signal old_scl        : std_logic;
signal buf_sda        : std_logic;
signal buf_scl        : std_logic;

signal command_vector : std_logic_vector(4 downto 0); -- concatenates all user commands in a single vector
signal ack_next_read  : std_logic;                      -- remembers (from commands) if next read should be ACK'd
signal shift_reg      : std_logic_vector(7 downto 0); -- the shift register to (de)serialize data
signal sdai           : std_logic;
signal scli           : std_logic;

begin

   command_vector <= send & read_with_ack & read_with_nack & repeat_start & stop;

   sda <= '0' when sdai = '0' else 'Z';
   scl <= '0' when scli = '0' else 'Z';


   process(clk)
   -- here, we control the sdai signal which is a local image of sda
   begin
      if rising_edge(clk) then
         case state is
            when send_byte =>
               sdai <= shift_reg(7);
            when send_ack =>
               sdai <= ack_next_read;
            when mk_start | start_send =>
               if time_over = '1' then
                  sdai <= '0';
               end if;
            when mk_stop =>
               if time_over = '1' and scli = '1' then
                  sdai <= '1';
               elsif scli = '0' then
                  sdai <= '0';
               end if;
            when others =>
               sdai <= '1';
         end case;
      end if;
   end process;




   process(clk)
   begin
      if rising_edge(clk) then
         case state is
            when send_byte | read_ack | read_byte | send_ack =>
               if time_over = '1' then
                  scli <= not scli;
               end if;
            when mk_start | start_send =>
               if time_over = '1' and sdai = '0' then
                  scli <= '0';
               end if;
            when pre_restart | mk_stop =>
               if time_over = '1' then
                  scli <= '1';
               end if;
            when bus_idle =>
               scli <= '0';
            when others =>
               scli <= '1';
         end case;
      end if;
   end process;



   FSM : process(clk)
   -- the module main state machine
   begin
      if rising_edge(clk) then
         if reset = '1' then
            state          <= bus_free;
            ack_next_read  <= '1';
         elsif old_sda = '0' and buf_sda /= '0' and old_scl /= '0' and buf_scl /= '0' then
            -- whatever happens, a stop condition sends to the bus_free state
            state <= bus_free;
         else
            case state is
               when bus_free   =>
                  -- the bus_free state is when nobody is working on the bus
                  if old_sda /= '0' and buf_sda = '0' and old_scl /= '0' and buf_scl /= '0' then
                     -- someone else is making a start
                     state <= bus_busy;
                  elsif buf_scl = '0' then
                     -- this one should not happen on I2C, Someone is making somthing unusual
                     state <= bus_exc;
                  elsif send = '1' then
                     state <= start_send;
                  elsif repeat_start = '1' then
                     state <= mk_start;
                  end if;
               
               when pre_restart =>
                  -- we must perform a restart, so first thing is to make sda='1'...
                  if time_over = '1' then
                     state <= mk_start;
                  end if;

               when mk_start | start_send => 
                  -- here, the master module is taking hand on the bus issuing a 'start' sequence
                  -- phase one   : sda = 1 / scl = 1
                  -- phase two   : sda = 0 / scl = 1
                  -- finally     : sda = 0 / scl = 0
                  if sdai = '1' and buf_sda = '0' then            -- actually, anoter master is performing START
                     state <= bus_busy;
                  elsif time_over = '1' and sdai = '0' then
                     if state = start_send then
                        state <= send_byte;
                     else
                        state <= bus_idle;
                     end if;
                  end if;
               
               when send_byte =>
                  -- here, the master sends something. It may be an address or data
                  if buf_scl /= '0' and sdai = '1' and buf_sda = '0' then -- except if someone else is speaking.
                     state <= bus_busy;
                  elsif remaining_bits = 0 and time_over = '1' and scli = '1' then
                     state <= read_ack;
                  end if;
               
               when read_ack =>
                  -- here, we wait for an ACK coming from the slave
                  if time_over = '1' and scli = '1' then
                     state <= bus_idle;
                  end if;
               
               when read_byte =>
                  -- here, we retreive data from slave
                  if remaining_bits = 0 and time_over = '1' and scli = '1' then
                     state <= send_ack;
                  end if;
               
               when send_ack =>
                  -- here, we send ACK, or NACK to the slave
                  if buf_scl /= '0' and sdai = '1' and buf_sda = '0' then -- except if someone else is speaking
                     state <= bus_busy;
                  elsif time_over = '1' and scli = '1' then
                     state <= bus_idle;
                  end if;
                  
               when mk_stop =>
                  -- here, we send a STOP to free the bus
                  -- phase one   : sda = 0 / scl = 0
                  -- phase two   : sda = 0 / scl = 1
                  -- finally     : sda = 1 / scl = 1
                  -- no need to set the next state, it will be done by the stop condition
                  -- test at the beginning of the process
               
               when bus_idle =>
                  -- in this state, the module waits for instructions and executes them
                  case command_vector is
                     when "10000" => -- send
                        state <= send_byte;
                     when "01000" => -- read with ACK
                        state <= read_byte;
                        ack_next_read <= '0';
                     when "00100" => -- read with NACK
                        state <= read_byte;
                        ack_next_read <= '1';
                     when "00010" => -- repeat start
                        state <= pre_restart;
                     when "00001" => -- stop
                        state <= mk_stop;
                     when others  => null;
                  end case;
               
               when bus_exc =>
                  if buf_scl /= '0' and buf_sda /= '0' then
                     state <= bus_free;
                  end if;
               when bus_busy  =>
                  -- here, another master is using the bus, so we just have to wait...
                  -- state will come back in bus_free state with the stop condition test
                  -- at the beginning of the process
                  null;
            end case;
         end if;
      end if;
   end process;



   time_counter : process(clk)
   -- here, we manage time_cnt to provide the expected frequency
   begin
      if rising_edge(clk) then
         case state is
            when bus_free =>
               time_cnt <= max_cnt;
            when mk_start | pre_restart | start_send | mk_stop =>
               if  time_over = '0' then
                  time_cnt <= time_cnt - 1;
               else
                  time_cnt <= max_cnt;
               end if;
            when read_byte | read_ack  | send_byte | send_ack  =>
               if (buf_scl /='0' or scli ='0') and time_over = '0' then  -- if clock is not stretched and counter has not expired
                  time_cnt <= time_cnt - 1;                           -- stretched clock only happen if we send '1' (scli = '1')
               else
                  time_cnt <= max_cnt;
               end if;               
            when others => null;
               time_cnt <= max_cnt;
         end case;
      end if;
   end process;


   timeout_flag : process(clk)
   begin
      if rising_edge(clk) then
         if time_cnt = 0 and time_over = '0' then
            time_over <= '1';
         else
            time_over <= '0';
         end if;
      end if;
   end process;


   process(clk)
   -- let's count the number of bits in the transferred byte
   begin
      if rising_edge(clk) then
         case state is
            when send_byte | read_byte =>
               if time_over = '1' and scli = '1' and remaining_bits > 0 then
                  remaining_bits <= remaining_bits - 1;
               end if;
            when others =>
               remaining_bits <= 7;
         end case;
      end if;
   end process;


   process(clk)
   -- here, we sample sda/scl for system stability and we remember the preious value
   begin
      if rising_edge(clk) then
         old_sda <= buf_sda;
         old_scl <= buf_scl;
         buf_sda <= sda;
         buf_scl <= scl;
      end if;
   end process;
   
   
   
   process(clk)
   -- we handle the shift register for both reading and writing
   begin
      if rising_edge(clk) then
         case state is
            when bus_free | bus_idle =>
               if command_vector = "10000" then
                  shift_reg <= data_in;
               end if;
            when send_byte | read_byte =>
               if time_over = '1' and scli = '1' then
                  shift_reg <= shift_reg(6 downto 0) & buf_sda;
               end if;
            when others =>
               null;
         end case;
      end if;
   end process;

   ready <= '1' when state = bus_free or state = bus_idle else '0';


   process(clk)
   -- when should we consider that an operation has been cancelled
   begin
      if rising_edge(clk) then
         case state is 
            when send_byte | send_ack =>
               if buf_scl /= '0' and sdai = '1' and buf_sda = '0' then        -- someone else speaking on the bus (including STOP from others)
                  cancelled <= '1';
               end if;
            when read_byte | read_ack =>                                      
               if old_scl /= '0' and buf_scl /= '0' and buf_sda /= '0' and old_sda = '0' then -- STOP during read or write
                  cancelled <= '1';
               end if;
            when mk_start | start_send =>
               if (sdai = '1'  and buf_sda = '0') or buf_scl='0' then  -- someone starts speaking before us
                  cancelled <= '1';
               end if;
            when others =>
               cancelled <= '0';
         end case;
      end if;
   end process;



   process(state, buf_sda, scli, shift_reg)
   begin
      if state = read_ack and scli = '1' then
         if buf_sda = '0' then
            data_out <= "00000000";
         else
            data_out <= "11111111";
         end if;
      else
         data_out <= shift_reg;
      end if;
   end process;

   process(state, scli, time_over)
   begin
      case state is 
         when read_ack | send_ack =>
            if time_over = '1' and scli = '1' then
               data_out_en <= '1';
            else
               data_out_en <= '0';
            end if;
         when others =>
            data_out_en <= '0';
      end case;
   end process;



end Behavioral;


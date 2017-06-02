-- fsm.vhd: Finite State Machine
-- Author(s): Lukas Pelanek
--
library ieee;
use ieee.std_logic_1164.all;
-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity fsm is
port(
   CLK         : in  std_logic;
   RESET       : in  std_logic;

   -- Input signals
   KEY         : in  std_logic_vector(15 downto 0);
   CNT_OF      : in  std_logic;

   -- Output signals
   FSM_CNT_CE  : out std_logic;
   FSM_MX_MEM  : out std_logic;
   FSM_MX_LCD  : out std_logic;
   FSM_LCD_WR  : out std_logic;
   FSM_LCD_CLR : out std_logic
);
end entity fsm;

-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of fsm is
   type t_state is (STATE1, STATE2, STATE3, STATE4, STATE5, STATE6A, STATE6B, STATE7A, STATE7B, STATE8A, STATE8B, STATE9A, STATE9B, STATE10A, STATE10B, PRINT_ERROR, BAD_PW, CORRECT_PW, PRINT_SUCCESS, FINISH);
   signal present_state, next_state : t_state;

begin
-- -------------------------------------------------------
sync_logic : process(RESET, CLK)
begin
   if (RESET = '1') then
      present_state <= STATE1;
   elsif (CLK'event AND CLK = '1') then
      present_state <= next_state;
   end if;
end process sync_logic;

-- -------------------------------------------------------
next_state_logic : process(present_state, KEY, CNT_OF)
begin
   case (present_state) is
   -- - - - - - - - - - - - - - - - - - - - - - -
   when STATE1 =>
      next_state <= STATE1;
		if (KEY(1) = '1') then
         next_state <= STATE2; 
		elsif (KEY(15) = '1') then
			next_state <= PRINT_ERROR; 
		elsif (KEY(14 downto 0) /= "000000000000000") then 
			next_state <= BAD_PW;
		end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when STATE2 =>
      next_state <= STATE2;
		if (KEY(1) = '1') then
         next_state <= STATE3; 
		elsif (KEY(15) = '1') then
			next_state <= PRINT_ERROR; 
		elsif (KEY(14 downto 0) /= "000000000000000") then 
			next_state <= BAD_PW;
		end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when STATE3 =>
      next_state <= STATE3;
		if (KEY(2) = '1') then
         next_state <= STATE4; 
		elsif (KEY(15) = '1') then
			next_state <= PRINT_ERROR; 
		elsif (KEY(14 downto 0) /= "000000000000000") then 
			next_state <= BAD_PW;
		end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when STATE4 =>
      next_state <= STATE4;
		if (KEY(4) = '1') then
         next_state <= STATE5; 
		elsif (KEY(15) = '1') then
			next_state <= PRINT_ERROR; 
		elsif (KEY(14 downto 0) /= "000000000000000") then 
			next_state <= BAD_PW;
		end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when STATE5 =>
      next_state <= STATE5;
		if (KEY(4) = '1') then
         next_state <= STATE6A; 
		elsif (KEY(1) = '1') then
         next_state <= STATE6B;
		elsif (KEY(15) = '1') then
			next_state <= PRINT_ERROR; 
		elsif (KEY(14 downto 0) /= "000000000000000") then 
			next_state <= BAD_PW;
		end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when STATE6A=>
      next_state <= STATE6A;
		if (KEY(3) = '1') then
         next_state <= STATE7A; 
		elsif (KEY(15) = '1') then
			next_state <= PRINT_ERROR; 
		elsif (KEY(14 downto 0) /= "000000000000000") then 
			next_state <= BAD_PW;
		end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when STATE6B=>
      next_state <= STATE6B;
		if (KEY(8) = '1') then
         next_state <= STATE7B; 
		elsif (KEY(15) = '1') then
			next_state <= PRINT_ERROR; 
		elsif (KEY(14 downto 0) /= "000000000000000") then 
			next_state <= BAD_PW;
		end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when STATE7A=>
      next_state <= STATE7A;
		if (KEY(7) = '1') then
         next_state <= STATE8A; 
		elsif (KEY(15) = '1') then
			next_state <= PRINT_ERROR; 
		elsif (KEY(14 downto 0) /= "000000000000000") then 
			next_state <= BAD_PW;
		end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when STATE7B=>
      next_state <= STATE7B;
		if (KEY(7) = '1') then
         next_state <= STATE8B; 
		elsif (KEY(15) = '1') then
			next_state <= PRINT_ERROR; 
		elsif (KEY(14 downto 0) /= "000000000000000") then 
			next_state <= BAD_PW;
		end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when STATE8A=>
      next_state <= STATE8A;
		if (KEY(3) = '1') then
         next_state <= STATE9A; 
		elsif (KEY(15) = '1') then
			next_state <= PRINT_ERROR; 
		elsif (KEY(14 downto 0) /= "000000000000000") then 
			next_state <= BAD_PW;
		end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when STATE8B=>
      next_state <= STATE8B;
		if (KEY(2) = '1') then
         next_state <= STATE9B; 
		elsif (KEY(15) = '1') then
			next_state <= PRINT_ERROR; 
		elsif (KEY(14 downto 0) /= "000000000000000") then 
			next_state <= BAD_PW;
		end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when STATE9A=>
      next_state <= STATE9A;
		if (KEY(0) = '1') then
         next_state <= STATE10A; 
		elsif (KEY(15) = '1') then
			next_state <= PRINT_ERROR; 
		elsif (KEY(14 downto 0) /= "000000000000000") then 
			next_state <= BAD_PW;
		end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when STATE9B=>
      next_state <= STATE9B;
		if (KEY(2) = '1') then
         next_state <= STATE10B; 
		elsif (KEY(15) = '1') then
			next_state <= PRINT_ERROR; 
		elsif (KEY(14 downto 0) /= "000000000000000") then 
			next_state <= BAD_PW;
		end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when STATE10A=>
      next_state <= STATE10A;
		if (KEY(7) = '1') then
         next_state <= CORRECT_PW; 
		elsif (KEY(15) = '1') then
			next_state <= PRINT_ERROR; 
		elsif (KEY(14 downto 0) /= "000000000000000") then 
			next_state <= BAD_PW;
		end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when STATE10B=>
      next_state <= STATE10B;
		if (KEY(1) = '1') then
         next_state <= CORRECT_PW; 
		elsif (KEY(15) = '1') then
			next_state <= PRINT_ERROR; 
		elsif (KEY(14 downto 0) /= "000000000000000") then 
			next_state <= BAD_PW;
		end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when CORRECT_PW =>
      next_state <= CORRECT_PW;
      if (KEY(15) = '1') then
         next_state <= PRINT_SUCCESS; 
		elsif (KEY(14 downto 0) /= "000000000000000") then 
			next_state <= BAD_PW;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when BAD_PW =>
      next_state <= BAD_PW;
      if (KEY(15) = '1') then
         next_state <= PRINT_ERROR; 
		else
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	
   when PRINT_ERROR =>
      next_state <= PRINT_ERROR;
      if (CNT_OF = '1') then
         next_state <= FINISH;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	 when PRINT_SUCCESS =>
      next_state <= PRINT_SUCCESS;
      if (CNT_OF = '1') then
         next_state <= FINISH;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	
   when FINISH =>
      next_state <= FINISH;
      if (KEY(15) = '1') then
         next_state <= STATE1; 
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when others =>
      next_state <= STATE1;
   end case;
end process next_state_logic;

-- -------------------------------------------------------
output_logic : process(present_state, KEY)
begin
   FSM_CNT_CE     <= '0';
   FSM_MX_MEM     <= '0';
   FSM_MX_LCD     <= '0';
   FSM_LCD_WR     <= '0';
   FSM_LCD_CLR    <= '0';

   case (present_state) is
   -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_SUCCESS =>
      FSM_CNT_CE     <= '1';
		FSM_MX_MEM		<= '1';
      FSM_MX_LCD     <= '1';
      FSM_LCD_WR     <= '1';
   -- - - - - - - - - - - - - - - - - - - - - - -
	when PRINT_ERROR =>
      FSM_CNT_CE     <= '1';
      FSM_MX_LCD     <= '1';
      FSM_LCD_WR     <= '1';
   -- - - - - - - - - - - - - - - - - - - - - - -
   when FINISH =>
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when others =>
		if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   end case;
end process output_logic;

end architecture behavioral;


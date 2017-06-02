-- cpu.vhd: Simple 8-bit CPU (BrainFuck interpreter)
-- Copyright (C) 2014 Brno University of Technology,
--                    Faculty of Information Technology
-- Author(s): Lukas Pelanek, xpelan03@stud.fit.vutbr.cz
--

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity cpu is
 port (
   CLK   : in std_logic;  -- hodinovy signal
   RESET : in std_logic;  -- asynchronni reset procesoru
   EN    : in std_logic;  -- povoleni cinnosti procesoru
 
   -- synchronni pamet RAM
   DATA_ADDR  : out std_logic_vector(12 downto 0); -- adresa do pameti
   DATA_WDATA : out std_logic_vector(7 downto 0); -- mem[DATA_ADDR] <- DATA_WDATA pokud DATA_EN='1'
   DATA_RDATA : in std_logic_vector(7 downto 0);  -- DATA_RDATA <- ram[DATA_ADDR] pokud DATA_EN='1'
   DATA_RDWR  : out std_logic;                    -- cteni (0) / zapis (1)
   DATA_EN    : out std_logic;                    -- povoleni cinnosti
   
   -- vstupni port
   IN_DATA   : in std_logic_vector(7 downto 0);   -- IN_DATA <- stav klavesnice pokud IN_VLD='1' a IN_REQ='1'
   IN_VLD    : in std_logic;                      -- data platna
   IN_REQ    : out std_logic;                     -- pozadavek na vstup data
   
   -- vystupni port
   OUT_DATA : out  std_logic_vector(7 downto 0);  -- zapisovana data
   OUT_BUSY : in std_logic;                       -- LCD je zaneprazdnen (1), nelze zapisovat
   OUT_WE   : out std_logic                       -- LCD <- OUT_DATA pokud OUT_WE='1' a OUT_BUSY='0'
 );
end cpu;


-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of cpu is

	-- Signaly pro praci s ukazatelem do pameti
	signal ptr    : std_logic_vector(12 downto 0):= "1000000000000";	-- Ukazatel do pameti inicializovan na hodnotu 4096 => musime se pohybovat v datove casti RAM
	signal ptr_inc   : std_logic:= '0';
	signal ptr_dec    : std_logic:= '0';
	signal ptr_getAdress   : std_logic:= '0';	
	
	-- Signaly pro praci s programovym citacem
   	signal pc    : std_logic_vector(12 downto 0);				-- Programovy citac inicializovan na hodnotu 0 => musime se pohybovat v casti RAM kde jsou ulozeny instrukce
	signal pc_inc    : std_logic := '0';
	signal pc_dec    : std_logic:= '0';
	signal pc_getAdress   : std_logic:= '0';
	
	-- Signaly pro praci s pocitadlem (vnorene cykly)
	signal cnt    : std_logic_vector(12 downto 0) := "0000000000000";	
	signal cnt_inc	: std_logic:= '0';
	signal cnt_dec	: std_logic:= '0';
	signal cnt_one		: std_logic:= '0';
	
	-- Signaly a stavy pro praci s instrukcemi
	type instruction_type is (incrementPointer, decrementPointer, incrementValue, decrementValue, startWhile, endWhile, print, read, halt, skip);
	signal instruction  : std_logic_vector(7 downto 0);
	signal instructionT  : instruction_type;
	signal instruction_read  : std_logic := '0';
	
	-- Stavy konecneho automatu
	type FSMstate is (idle, fetch, fetch1, decodeInstr, haltcpu, incVal, decVal, beginWhile, quitWhile, printVal, readVal, whileStep, whileStep1, whileStep2, whileStep3, quitWhile2, beginWhile2);
	signal pState    : FSMstate;
	signal nState    : FSMstate;
---------------------------------------------------------------------------------
begin

-- Musim rozlisovat, zda chci pracovat s daty nebo instrukcemi, proto na adresovou sbernici posilam adresu pomoci aktivniho signalu pc_getAdress nebo ptr_getAdress
DATA_ADDR <= pc when pc_getAdress='1' else
		(others => 'Z');

DATA_ADDR <= ptr when ptr_getAdress='1' else
		(others => 'Z');


-- Proces zabezpecujici chod program counteru
PROGRAM_COUNTER: process(CLK, RESET)
	begin
		if (RESET = '1') then 
			pc <= (others=>'0');		
		elsif (CLK'event) and (CLK = '1') then 
			if (pc_dec = '1') then 
				pc <= pc - 1;
            			pc(12) <= '0';		-- program counter se musi pohybovat v intervalu <0, 4095>, proto nejvyznamnejsi by nastavuji na 0
			elsif (pc_dec = '0') and (pc_inc = '1') then 
				pc <= pc + 1;
          		        pc(12) <= '0';
			end if;
		end if;	
	end process; 

-- Proces zabezpecujici chod ukazatele
POINTER: process(CLK, RESET)
	begin
		if (RESET = '1') then 
			ptr <= "1000000000000";		
		elsif (RESET = '0') and (CLK'event) and (CLK = '1') then 
			if (ptr_dec = '1') then 
				ptr <= ptr - 1;
				ptr(12) <= '1';		-- stejny pripad jako u program counteru, avsak ted se musim pohybovat v intervalu <4096, 8191>
			
			elsif (ptr_inc= '1') then 
				ptr <= ptr + 1;
				ptr(12) <= '1';
			end if;
		end if;	
	end process;

-- Proces zabezpecujici chod counteru
COUNTER: process(CLK, RESET)
	begin
		if (RESET = '1') then 
			cnt <= (others=>'0');
		elsif (CLK'event) and (CLK = '1') then 
			if (cnt_inc = '1') then
				cnt <= cnt + 1;
			elsif (cnt_dec = '1') then
				cnt <= cnt - 1;
			elsif (cnt_one='1') then
				cnt <= "0000000000001";
			end if;
		end if;	
	end process;

-- Proces nacitajici instrukce
INSTRUCTIONS: process(RESET, CLK)
	begin
		if (RESET='1') then 
			 instruction <= (others=>'0');		
		elsif (CLK'event) and (CLK='1') then
			if (instruction_read = '1') then
				instruction <= DATA_RDATA; --ulozeni dat z ram pameti do pomocneho signalu
			end if;
		end if;
	end process;


-- Proces dekodujici instrukce
DECODE: process(instruction) 
	begin
		case instruction is
			when X"2B" => instructionT <= incrementValue;  
			when X"2D" => instructionT <= decrementValue;  
			when X"3E" => instructionT <= incrementPointer;  
			when X"3C" => instructionT <= decrementPointer;  
			when X"5B" => instructionT <= startWhile;  
			when X"5D" => instructionT <= endWhile;  
			when X"2E" => instructionT <= print;  
			when X"2C" => instructionT <= read;  
			when X"00" => instructionT <= halt;  
			when others => instructionT <= skip; 
		end case;
	end process;
	
-- Proces zajistujici zmenu stavu automatu
FSM_STATES: process(EN, RESET, CLK)
	begin
	  if (RESET='1') then
		 pState <= idle;
		elsif (CLK'event) and (CLK='1') then
		if(EN = '1') then
			pState <= nState; 
		end if;
	  end if;
	end process;

-- Konecny automat 	
FSM: process(DATA_RDATA, OUT_BUSY, IN_VLD, IN_DATA, cnt, pState, instruction, instructionT)
	
	begin
      	nState <= idle;

	-- nastavim vsechny signaly na nulu a pote v prislusnem stavu si je nastavim podle potreby
	OUT_DATA <= "00000000";
	DATA_WDATA <= "00000000";
	IN_REQ <= '0';
	DATA_RDWR <= '0';
    	DATA_EN <= '0';
   	OUT_WE <= '0';
     	ptr_dec <= '0';
	ptr_inc<= '0';
	ptr_getAdress <= '0';
	pc_getAdress <= '0';
 	instruction_read<= '0';
	pc_dec <= '0';
	pc_inc <= '0';
	cnt_one <= '0';
	cnt_inc <= '0';
	cnt_dec <= '0';
	
case pState is	
				-- dekodovani instrukci + priprava provedeni instrukci/vykonani jednoduchych instrukci (inkrementace/dekrementace ukazatele)
			when decodeInstr =>
				case instructionT is
					when halt =>
						nState <= haltcpu;

					when incrementPointer =>
						ptr_inc<= '1';
						pc_inc <= '1'; 

						nState <= fetch;

					when decrementPointer =>
						ptr_dec <= '1';
						pc_inc <= '1'; 

						nState <= fetch;
						
					when incrementValue =>
						DATA_EN <= '1'; 
						ptr_getAdress <= '1'; 				
						DATA_RDWR <= '0'; 			
				
						nState <= incVal;
						
					when decrementValue =>
						DATA_EN <= '1'; 
						ptr_getAdress <= '1'; 			
						DATA_RDWR <= '0'; 			
				
						nState <= decVal;
						
					when startWhile =>
						DATA_EN <= '1';
						DATA_RDWR <= '0';
						ptr_getAdress <= '1';

						nState <= beginWhile;
						
					when endWhile =>
						DATA_EN <= '1';
						DATA_RDWR <= '0';
						ptr_getAdress <= '1';

						nState <= quitWhile;
						
					when print =>
						DATA_EN <= '1'; 
						ptr_getAdress <= '1'; 				
						DATA_RDWR <= '0'; 			
				
						nState <= printVal;

					when read =>
						IN_REQ <= '1';		
	
						nState <= readVal;
						
					when skip =>
						pc_dec <= '0';
						pc_inc <= '1'; 
				
						nState <= fetch;
				end case;

				-- pozastaveni procesoru
			when haltcpu =>
				nState <= haltcpu;
	
				-- nedelej nic
			when idle =>
				nState <= fetch;

				-- priprava nacteni instrukce
			when fetch => 
				nState <= fetch1;
				pc_getAdress <= '1';
				DATA_EN <= '1';

				-- nacteni instrukce
			when fetch1 => 
				instruction_read<= '1';

				nState <= decodeInstr;	

				-- inkrementace aktualni bunky
			when incVal => 
					ptr_getAdress <= '1'; 
					DATA_EN <= '1'; 
					DATA_RDWR <= '1'; 
					DATA_WDATA <= DATA_RDATA + 1;
		
					pc_inc <= '1'; 

					nState <= fetch;

				-- dekrementace aktualni bunky
			when decVal => 
					ptr_getAdress <= '1'; 
					DATA_EN <= '1'; 
					DATA_RDWR <= '1'; 
					DATA_WDATA <= DATA_RDATA - 1;
					pc_inc <= '1'; 

					nState <= fetch;
						
				-- tisk aktualni bunky
			when printVal => 
					if (OUT_BUSY = '0') then 
						OUT_WE <= '1';
						OUT_DATA <= DATA_RDATA ;
						pc_inc <= '1'; 
						nState <= fetch;	
					else 
						nState <= printVal;
				end if;		
					
				-- precte hodnotu z klavesnice do aktualni bunky
			when readVal => 
				if (IN_VLD = '1') then
					DATA_EN <= '1'; 
					ptr_getAdress <= '1'; 			
					DATA_RDWR <= '1'; 
					DATA_WDATA <= IN_DATA;
					pc_inc <= '1';

					nState <= fetch;	
				else 
					nState <= readVal;
				end if;	
					
				-- zacatek cyklu
			when beginWhile =>
				if(DATA_RDATA= "00000000") then
					pc_inc <= '1';
					cnt_inc <= '1';
					DATA_EN <= '1';

					nState <= whileStep;
				else
					pc_inc <= '1';

					nState <= fetch;
				end if;	
				
				-- kontrola vnorenych cyklu
			when beginWhile2 =>
				if (cnt= "0000000000000") then
					nState <= fetch;
				elsif (instruction = "000001011101") then
					cnt_dec <= '1';
					pc_inc <= '1';
					DATA_EN <= '1';

					nState <= whileStep;
				elsif (instruction = "000001011011") then
					cnt_inc <= '1';
					pc_inc<= '1';
					DATA_EN <= '1';

					nState <= whileStep;
				else
					pc_inc <= '1';
					DATA_EN <= '1';

					nState <= whileStep;
				end if;			
				
				-- mezistav cyklu
			when whileStep =>
				DATA_EN <= '1';
				pc_getAdress <= '1';

				nState <= whileStep1;
				
				-- mezistav cyklu
			when whileStep1 =>	  
				instruction_read<= '1';

				nState <= beginWhile2;
			
				-- konec cyklu	
			when quitWhile =>				
				if(DATA_RDATA= "00000000") then
					pc_inc <= '1';

					nState <= fetch;
				else
					cnt_one <= '1';
					pc_dec <= '1';

					nState <= whileStep2;
				end if;			
				
				-- kontrola vnorenych cyklu
			when quitWhile2 =>	  
				if (instruction = "000001011101") then
					cnt_inc <= '1';
					pc_dec <= '1';

					nState <= whileStep2;
					DATA_EN <= '1';
				elsif (instruction = "000001011011") and (cnt = "000000000001")then
					cnt_dec <= '1';

					nState <= fetch;
				elsif (instruction = "000001011011") and (cnt /= "000000000001")then
					cnt_dec <= '1';
					pc_dec <= '1';
					DATA_EN <= '1';

					nState <= whileStep2;
				else
					pc_dec <= '1';
					DATA_EN <= '1';

					nState <= whileStep2;
				end if;		
	
				-- mezistav cyklu
			when whileStep2 =>
				pc_getAdress <= '1';
				DATA_EN <= '1';

				nState <= whileStep3;
				
				-- mezistav cyklu
			when whileStep3 =>	  
				instruction_read<= '1';

				nState <= quitWhile2;
			
			when others => 
				nState <= idle;
		end case;
	end process;
end behavioral;   

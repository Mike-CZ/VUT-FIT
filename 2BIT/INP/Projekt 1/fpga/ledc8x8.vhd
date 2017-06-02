--Prvni projekt do predmetu INP
--Lukas Pelanek
--xpelan03

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_unsigned.all;
use IEEE.std_logic_arith.all;

entity ledc8x8 is
port (

    LED: out std_logic_vector(0 to 7);
    SMCLK: in std_logic;
    RESET: in std_logic;
    ROW: out std_logic_vector(0 to 7)

);
end ledc8x8;


architecture behav of ledc8x8 is

signal myReg: std_logic_vector(7 downto 0);
signal myRow: std_logic_vector(7 downto 0);
signal ce: std_logic;

begin

    process(RESET, SMCLK, myReg, ce)   
  
    begin
        if RESET = '1' then
            myReg <= "00000000";
        elsif SMCLK'event and SMCLK = '1' then
            myReg <= myReg + 1;
        end if;      
       ce <= myReg(7);               --Deleni frekvence

    end process;

    process(RESET, ce)

    begin
        if RESET = '1' then
            myRow <= "10000000";
        elsif ce'event AND ce = '1' then

           	case myRow is   --Dekoder pro vyber radku      
				when "10000000" => myRow <= "01000000";
				when "01000000" => myRow <= "00100000";
				when "00100000" => myRow <= "00010000";
				when "00010000" => myRow <= "00001000";
				when "00001000" => myRow <= "00000100";
				when "00000100" => myRow <= "00000010";
				when "00000010" => myRow <= "00000001";
				when "00000001" => myRow <= "10000000";	
				when others => myRow <= "10000000";
			end case;
        end if;

    end process;
    
    
    process(myRow)

    begin

        case myRow is      --Dekoder rozsvicujici diody
            when "10000000" =>  LED <= "01110000";
            when "01000000" =>  LED <= "01110110";
            when "00100000" =>  LED <= "01110110";
            when "00010000" =>  LED <= "01110000";
            when "00001000" =>  LED <= "01110111";
            when "00000100" =>  LED <= "01110111";
            when "00000010" =>  LED <= "01110111"; 
            when "00000001" =>  LED <= "00000111";
            when others    =>  LED <= "11111111"; 
        end case;

    end process;
    
    ROW <= myRow; 

end behav;


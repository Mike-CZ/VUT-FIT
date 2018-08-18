# Soubor:  Makefile
# Datum:   2018/4/28
# Autor:   Bc. Lukas Pelanek, xpelan03@stud.fit.vutbr.cz
# Projekt: KRY, Druhy projekt

CC =g++
CFLAGS =-pedantic -Wextra -std=c++17 -O3
FILES =Main.cpp Functions.cpp RSA.cpp
PROGRAM =kry

all:
	$(CC) $(CFLAGS) $(FILES) -o $(PROGRAM) -lgmp -lgmpxx

clean:
	rm $(PROGRAM)
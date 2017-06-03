  # POS - Pokrocile operacni systemy
  # 17.2.2017
  # Autor: Bc. Lukas Pelanek, xpelan03@stud.fit.vutbr.cz

PROGRAM=program
SRC=xpelan03.c

CC=gcc
CFLAGS=-Wall -g -pedantic -std=c11 -pthread

all:  clean ${PROGRAM} 

${PROGRAM}: 
	$(CC) $(CFLAGS) $(SRC) -o $@

clean:
	rm -f *.o ${PROGRAM}


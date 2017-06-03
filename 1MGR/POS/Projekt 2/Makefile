  # POS - Pokrocile operacni systemy
  # 23.4.2017
  # Autor: Bc. Lukas Pelanek, xpelan03@stud.fit.vutbr.cz

PROGRAM=proj2
SRC=proj2.c

CC=gcc
CFLAGS=-Wall -g -pedantic -std=c11 -pthread

all:  clean ${PROGRAM} 

${PROGRAM}: 
	$(CC) $(CFLAGS) $(SRC) -o $@

clean:
	rm -f *.o ${PROGRAM}


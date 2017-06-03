# Soubor:  Makefile
# Datum:   2017/4/20
# Autor:   Bc. Lukas Pelanek, xpelan03@stud.fit.vutbr.cz
# Projekt: PDS, Man in the Middle

CC =g++
CFLAGS =-pedantic -Wextra -std=c++11
HANDLER =IOHandler.cpp

all: pds-scanner pds-chooser pds-spoof pds-intercept

pds-scanner:
	$(CC) $(CFLAGS) Scanner.cpp $(HANDLER) -o pds-scanner

pds-chooser:
	$(CC) $(CFLAGS) Chooser.cpp $(HANDLER) -o pds-chooser

pds-spoof:
	$(CC) $(CFLAGS) Spoofer.cpp $(HANDLER) -o pds-spoof

pds-intercept:
	$(CC) $(CFLAGS) Intercepter.cpp $(HANDLER) -o pds-intercept

clean:
	rm pds-scanner pds-chooser pds-spoof pds-intercept
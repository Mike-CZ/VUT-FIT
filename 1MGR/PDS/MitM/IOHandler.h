/*
* Soubor:  IOHandler.h
* Datum:   2017/4/20
* Autor:   Bc. Lukas Pelanek, xpelan03@stud.fit.vutbr.cz
* Projekt: PDS, Man in the Middle
* Popis:   Sada funkci pro praci se vstupnimi a vystupnimi daty
*/


#ifndef IOHANDLER_H
#define IOHANDLER_H

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <getopt.h>


/* Struktura pro praci s parametry scanneru */
typedef struct {
	std::string interface;
	std::string file;
} t_scannerParams;

/* Struktura pro praci s parametry spooferu */
typedef struct {
	std::string interface;
	std::string victimIP;
	std::string victimMAC;
	std::string victimIP2;
	std::string victimMAC2;
	std::string protocol;
	int timer;
} t_spooferParams;

/*
* Sada funkci pro praci se vstupnimi parametry, vstupnimi soubory a stdout
*/
namespace IOHandler {
	enum Errors {
		ERR_NO_ERROR = 0,
		ERR_ARGUMENT_COUNT = -1,
		ERR_UNKNOWN_ARGUMENT = -2,
		ERR_OUT_FILE = -3,
		ERR_ARP_SEND = -4,
		ERR_IPV6 = -5,
		ERR_NO_IF_FOUND = -6,
		ERR_INIT = -7,
		ERR_ARP_SPOOF = -8,
		ERR_NDP_SPOOF = -9,
		ERR_IN_FILE = -10,
		ERR_INTERCEPT = -11
	};

	/* Funkce nacte vstupni parametry pro scanner */
	int getScannerParameters(int argc, char **argv, t_scannerParams *params);

	/* Funkce nacte vstupni parametry pro spofer */
	int getSpooferParameters(int argc, char **argv, t_spooferParams *params);

	/* Funkce nacte vstupni parametry pro Chooser */
	int getChooserParameters(int argc, char **argv, t_scannerParams *params);

	/* Tisk chybove hlasky */
	void printError(int errCode);

	/* Otevre soubor pro zapis */
	int openOutputFile(std::string fileName, std::ofstream *stream);

	/* Zavre soubor pro zapis */
	void closeOutputFile(std::ofstream *stream);

}

#endif
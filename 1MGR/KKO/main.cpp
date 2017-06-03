/*
 * Autor: Lukas pelanek (xpelan03)
 * Datum: 29.4.2017
 * Soubor: main.cpp
 * Komentar: Hlavni telo programu
 */ 

#include <getopt.h>
#include "ahed.h"

enum e_Action {
	A_CODE = 1,
	A_DECODE = 2
};

int getParams(int argc, char **argv, std::string *inFile, std::string *outFile, std::string *logFile, e_Action *action);

void printhelp() {
	std::cout << "Aplikace realizujici huffmanovo kodovani a dekodovani" << std::endl;
	std::cout << "Autor:\tLukas Pelanek" << std::endl;
	std::cout << "Login:\txpelan03" << std::endl;
	std::cout << "Spusteni:\t./ahed -i <file> -o <file> -l <file> -c|-x" << std::endl;
	std::cout << "-i:\tVstupni soubor" << std::endl;
	std::cout << "-o:\tVystupni soubor" << std::endl;
	std::cout << "-l:\tLogovaci soubor" << std::endl;
	std::cout << "-c:\tKodovani" << std::endl;
	std::cout << "-x:\tDekodovani" << std::endl;
}

/* Hlavni telo */
int main(int argc, char **argv) {
	std::string inFile, outFile, logFile;
	e_Action action;
	tAHED record;
	FILE *input;
	FILE *output;
	FILE *logfile;
	int err;

	err = 0;
	input = NULL;
	output = NULL;
	logfile = NULL;

	// nacteni vstupnich parametru
	if ((err = getParams(argc, argv, &inFile, &outFile, &logFile, &action)) != 0) {
		return AHEDFail;
	}

	// otevreni vstupniho souboru
	if (inFile.length() > 0) {
		if ((input = std::fopen(inFile.c_str(), "rb")) == NULL) {
			std::cerr << "Chyba pri otevirani vstupniho souboru!" << std::endl;
			return AHEDFail;
		}
	}
	else {
		input = stdin;
	}
	
	// otevreni vystupniho souboru
	if (outFile.length() > 0) {
		if ((output = std::fopen(outFile.c_str(), "wb")) == NULL) {
			std::fclose(input);
			std::cerr << "Chyba pri otevirani vystupniho souboru!" << std::endl;
			return AHEDFail;
		}
	}
	else {
		output = stdout;
	}

	// otevreni logovaciho souboru
	if (logFile.length() > 0) {
		if ((logfile = std::fopen(logFile.c_str(), "wb")) == NULL) {
			std::cerr << "Chyba pri otevirani logovaciho souboru!" << std::endl;
			return -1;
		}
	}

	// kodovani
	if (action == A_CODE) {
		if ((err = AHEDEncoding(&record, input, output)) != AHEDOK) {
			std::fclose(input);
			std::fclose(output);
			std::fclose(logfile);
			std::cerr << "Chyba pri kodovani!" << std::endl;
			return AHEDFail;
		}
	}

	// dekodovani
	else if (action == A_DECODE) {
		if ((err = AHEDDecoding(&record, input, output)) != AHEDOK) {
			std::fclose(input);
			std::fclose(output);
			std::fclose(logfile);
			std::cerr << "Chyba pri dekodovani!" << std::endl;
			return AHEDFail;
		}
	}

	// vypis logu
	if (logfile != NULL) {
		fprintf(logfile, "login = xpelan03\n");
		fprintf(logfile, "uncodedSize = %lld\n", record.uncodedSize);
		fprintf(logfile, "codedSize = %lld\n", record.codedSize);
		std::fclose(logfile);
	}

	std::fclose(input);
	std::fclose(output);

	return 0;
}


/* Nacteni vstupnich parametru */
int getParams(int argc, char **argv, std::string *inFile, std::string *outFile, std::string *logFile, e_Action *action) {
	int opt;

	while ((opt = getopt(argc, argv, "i:o:l:cxh")) != -1) {
		switch (opt) {
		case 'i':
			*inFile = optarg;
			break;
		case 'o':
			*outFile = optarg;
			break;
		case 'l':
			*logFile = optarg;
			break;
		case 'c':
			if (*action == A_DECODE) {
				std::cerr << "Muzete vybrat pouze jednu operaci. Zda chcete data kodovat ci dekodovat!" << std::endl;
				return -1;
			}
			*action = A_CODE;
			break;
		case 'x':
			if (*action == A_CODE) {
				std::cerr << "Muzete vybrat pouze jednu operaci. Zda chcete data kodovat ci dekodovat!" << std::endl;
				return -1;
			}
			*action = A_DECODE;
			break;
		case 'h':
			printhelp();
			exit(0);
		case '?':
			std::cerr << "Neznamy parametr!" << std::endl;
			return -1;
		}
	}

	if (*action != A_CODE && *action != A_DECODE) {
		std::cerr << "Musite zadat, zda chcete data kodovat ci dekodovat!" << std::endl;
		return -1;
	}

	return 0;
}
	

/*
* Soubor:  bms2A.cpp
* Datum:   2017/12/02
* Autor:   Bc. Lukas Pelanek, xpelan03@stud.fit.vutbr.cz
* Projekt: BMS 2
* Popis:   QPSK modulator.
*/

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <cstddef>

#include "sndfile.hh"

// constants
#define SAMPLE_RATE 18000
#define CHANELS 1
#define FORMAT (SF_FORMAT_WAV | SF_FORMAT_PCM_24)
#define AMPLITUDE (1.0 * 0x7F000000)
#define FREQ (1000.0 / SAMPLE_RATE)

// baud rate and baud size (bit rate = baud_rate * 2)
#define BAUD_RATE 600
#define BAUD_SIZE (SAMPLE_RATE / BAUD_RATE)

// phases
#define PHASE_01 M_PI / 4
#define PHASE_00 3 * (M_PI / 4) 
#define PHASE_10 5 * (M_PI / 4) 
#define PHASE_11 7 * (M_PI / 4) 

// phases and their shifts
typedef enum {
	T_PHASE_01 = 1,
	T_PHASE_00 = 2,
	T_PHASE_10 = 3,
	T_PHASE_11 = 4
} tPhases;

// read input symbols
int getInputSymbols(std::string fileName, std::vector<tPhases> *inputSymbols) {
	std::ifstream infile(fileName);

	if (!infile.is_open()) {
		std::cerr << "Nepodarilo se otevrit vstupni soubor." << std::endl;
		return -1;
	}

	char c;
	int i = 0;
	bool isZeroPrefixed = false;

	while (infile >> c) {

		// validate input symbol
		if (c != '0' && c != '1') {
			std::cerr << "Vstupni soubor musi obsahovat pouze znaky 0 nebo 1." << std::endl;
			infile.close();
			return -1;
		}

		if (i % 2 == 1) {
			if (isZeroPrefixed) {
				inputSymbols->push_back(c == '0' ? T_PHASE_00 : T_PHASE_01);
			}
			else {
				inputSymbols->push_back(c == '0' ? T_PHASE_10 : T_PHASE_11);
			}
		}
		// set flag
		else {
			isZeroPrefixed = c == '0' ? true : false;
		}
		i++;
	}

	infile.close();

	// check parity
	if (i % 2 != 0) {
		std::cerr << "Pocet vstupnich dat musi byt sudy." << std::endl;
		return -1;
	}

	// check if data were read
	if (inputSymbols->size() == 0) {
		std::cerr << "Vstupni soubor neobsahuje zadna data." << std::endl;
		return -1;
	}
	
	return 0;
}

// modulate
int modulate(std::vector<tPhases> *inputSymbols, SndfileHandle *outputFile) {

	int *buffer = new int[SAMPLE_RATE];
	int i = 0;

	// add sync squence
	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < BAUD_SIZE; y++) {
			buffer[i] = AMPLITUDE * sin(2 * M_PI * FREQ * i + (x % 2 == 0 ? PHASE_00 : PHASE_11));
			i++;
		}
	}

	for (auto &item : *inputSymbols) {
		
		for (int x = 0; x < BAUD_SIZE; x++) {

			// store phase into buffer
			switch (item) {
				case T_PHASE_00:
					buffer[i % SAMPLE_RATE] = AMPLITUDE * sin(2 * M_PI * FREQ * i + PHASE_00);
					break;
				case T_PHASE_01:
					buffer[i % SAMPLE_RATE] = AMPLITUDE * sin(2 * M_PI * FREQ * i + PHASE_01);
					break;
				case T_PHASE_10:
					buffer[i % SAMPLE_RATE] = AMPLITUDE * sin(2 * M_PI * FREQ * i + PHASE_10);
					break;
				case T_PHASE_11:
					buffer[i % SAMPLE_RATE] = AMPLITUDE * sin(2 * M_PI * FREQ * i + PHASE_11);
					break;
				default:
					std::cerr << "Doslo k chybe pri modulaci." << std::endl;
					delete[] buffer;
					return -1;
			}

			i++;

			// write data when buffer full
			if (i % SAMPLE_RATE == 0) {
				outputFile->write(buffer, SAMPLE_RATE);
			}
		}
	}

	// write data
	if (i % SAMPLE_RATE > 0) {
		outputFile->write(buffer, i % SAMPLE_RATE);
	}

	delete[] buffer;

	return 0;
}

int main(int argc, char** argv) {

    SndfileHandle outputFile;
	std::vector<tPhases> inputSymbols;

	// validate number of parameters
	if (argc != 2) {
		std::cerr << "Spatny pocet parametru. Spusteni aplikace: ./bms1A nazov_suboru.txt" << std::endl;
		return EXIT_FAILURE;
	}

	// get new file name
	std::string filename(argv[1]);
	std::size_t found = filename.find_last_of(".");
	if (found != std::string::npos) {
		filename = filename.substr(0, found);
	}
	filename += ".wav";

	// get input symbols
	if (getInputSymbols(argv[1], &inputSymbols) < 0) {
		return EXIT_FAILURE;
	}

	// set output file
	outputFile = SndfileHandle(filename, SFM_WRITE, FORMAT, CHANELS, SAMPLE_RATE);

	// modulate
	if (modulate(&inputSymbols, &outputFile) < 0) {
		return EXIT_FAILURE;
	}
	
    return EXIT_SUCCESS;
}


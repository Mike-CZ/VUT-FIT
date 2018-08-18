/*
* Soubor:  bms2B.cpp
* Datum:   2017/12/02
* Autor:   Bc. Lukas Pelanek, xpelan03@stud.fit.vutbr.cz
* Projekt: BMS 2
* Popis:   QPSK demodulator.
*/

#include <cstdlib>
#include <string>
#include <fstream>
#include <iostream>
#include <math.h>

#include "sndfile.hh"

// constants
#define AMPLITUDE (1.0 * 0x7F000000)
#define FREQ 1000.0

// phases
#define PHASE_01 M_PI / 4
#define PHASE_00 3 * (M_PI / 4) 
#define PHASE_10 5 * (M_PI / 4) 
#define PHASE_11 7 * (M_PI / 4) 

// threshold for sampling
#define THRESHOLD (AMPLITUDE / 200000)

// max difference between baud lengths
#define BAUD_THRESHOLD 2


// get baud size from sync sequence 00110011
int getBaudSize(int *buffer, int dataLength, double frequency) {
	int firstBaudLengt = 0;
	bool firstBaudFinished = false;

	int secondBaudLengt = 0;
	bool secondBaudFinished = false;

	int thirdBaudLengt = 0;
	bool thirdBaudFinished = false;

	int fourthBaudLengt = 0;
	bool fourthBaudFinished = false;

	int value = 0;
	int i = -1;

	for (int x = 0; x < dataLength; x++) {
		i++;

		// get length of first baud
		if (!firstBaudFinished) {
			value = AMPLITUDE * sin(2 * M_PI * frequency * i + PHASE_00);
			if (!(abs(value - buffer[x]) > THRESHOLD)) {
				firstBaudLengt++;
				continue;
			}
			firstBaudFinished = true;
		}

		// get length of second baud
		if (!secondBaudFinished) {
			value = AMPLITUDE * sin(2 * M_PI * frequency * i + PHASE_11);
			if (!(abs(value - buffer[x]) > THRESHOLD)) {
				secondBaudLengt++;
				continue;
			}
			secondBaudFinished = true;
		}

		// get length of third baud
		if (!thirdBaudFinished) {
			value = AMPLITUDE * sin(2 * M_PI * frequency * i + PHASE_00);
			if (!(abs(value - buffer[x]) > THRESHOLD)) {
				thirdBaudLengt++;
				continue;
			}
			thirdBaudFinished = true;
		}

		// get length of fourth baud
		if (!fourthBaudFinished) {
			value = AMPLITUDE * sin(2 * M_PI * frequency * i + PHASE_11);
			if (!(abs(value - buffer[x]) > THRESHOLD) && fourthBaudLengt < firstBaudLengt) {	// end sync sequence if equal to first (because the input can begin with 11...)
				fourthBaudLengt++;
				continue;
			}
			fourthBaudFinished = true;
		}

		if (firstBaudFinished && secondBaudFinished && thirdBaudFinished && fourthBaudFinished) {
			break;
		}
	}

	// check if bauds were succesfully read
	if (! (firstBaudFinished && secondBaudFinished && thirdBaudFinished && fourthBaudFinished)) {
		std::cerr << "Chyba pri synchronizaci. Nepodarilo se detekovat synchronizacni sekvenci na zacatku souboru." << std::endl;
		return -1;
	}

	// validate difference between baud lengths
	if (abs(firstBaudLengt - secondBaudLengt) > BAUD_THRESHOLD ||
		abs(firstBaudLengt - thirdBaudLengt) > BAUD_THRESHOLD ||
		abs(secondBaudLengt - fourthBaudLengt) > BAUD_THRESHOLD ||
		abs(thirdBaudLengt - fourthBaudLengt) > BAUD_THRESHOLD ||
		abs(firstBaudLengt - fourthBaudLengt) > BAUD_THRESHOLD) {
		std::cerr << "Chyba pri synchronizaci. Nepodarilo se detekovat velikost baudu." << std::endl;
		return -1;
	}

	// get total length
	int totalLength = firstBaudLengt + secondBaudLengt + thirdBaudLengt + fourthBaudLengt;

	return (int) round((double)totalLength / 4);
}

// demodulate input signal
int demodulate(SndfileHandle *inputFile, int *buffer, int dataLength, double frequency, int baudSize, std::string *out) {
	int *bufferPtr;
	int value = 0;
	int i = 0;

	// clear output string
	out->clear();

	// move pointer because of sync
	bufferPtr = &(buffer[baudSize * 4]);
	dataLength = dataLength - (baudSize * 4);
	i = baudSize * 4;

	// number of values those matched in one baud
	int phase00 = 0;
	int phase01 = 0;
	int phase10 = 0;
	int phase11 = 0;

	do {

		for (int x = 0; x <= dataLength; x++) {

			// baud was read
			if (x % baudSize == 0 && x > 0) {

				// find the most matched phase and print binary data
				if (phase00 > phase01 && phase00 > phase10 && phase00 > phase11) {
					out->append("00");
				}
				else if (phase01 > phase00 && phase01 > phase10 && phase01 > phase11) {
					out->append("01");
				}
				else if (phase10 > phase00 && phase10 > phase01 && phase10 > phase11) {
					out->append("10");
				}
				else if (phase11 > phase00 && phase11 > phase01 && phase11 > phase10) {
					out->append("11");
				}
				else {
					std::cerr << "Nepodarilo se demodulovat data." << std::endl;
					return -1;
				}

				// reset counters
				phase00 = 0;
				phase01 = 0;
				phase10 = 0;
				phase11 = 0;
			}

			// read data
			if (x < dataLength) {

				// check if matches phase for 00
				value = AMPLITUDE * sin(2 * M_PI * frequency * i + PHASE_00);
				if (!(abs(value - bufferPtr[x]) > THRESHOLD)) {
					phase00++;
				}

				// check if matches phase for 01
				value = AMPLITUDE * sin(2 * M_PI * frequency * i + PHASE_01);
				if (!(abs(value - bufferPtr[x]) > THRESHOLD)) {
					phase01++;
				}

				// check if matches phase for 10
				value = AMPLITUDE * sin(2 * M_PI * frequency * i + PHASE_10);
				if (!(abs(value - bufferPtr[x]) > THRESHOLD)) {
					phase10++;
				}

				// check if matches phase for 11
				value = AMPLITUDE * sin(2 * M_PI * frequency * i + PHASE_11);
				if (!(abs(value - bufferPtr[x]) > THRESHOLD)) {
					phase11++;
				}

				i++;
			}
		}

		// set buffer pointer back
		bufferPtr = buffer;

	} while ((dataLength = inputFile->read(buffer, inputFile->samplerate())) > 0);

	return 0;
}


int main(int argc, char** argv) {
    
    SndfileHandle inputFile;
    int sampleRate;
    int *buffer;
	double frequency;
	int dataLength;
	int baudSize;
	std::string output;

	// validate number of parameters
	if (argc != 2) {
		std::cerr << "Spatny pocet parametru. Spusteni aplikace: ./bms2B nazev_souboru.wav" << std::endl;
		return EXIT_FAILURE;
	}

	// get new file name
	std::string filename(argv[1]);
	std::size_t found = filename.find_last_of(".");
	if (found != std::string::npos) {
		filename = filename.substr(0, found);
	}
	filename += ".txt";
    
	// create input file handler
    inputFile = SndfileHandle(argv[1]);
    
	// get sample rate
    sampleRate = inputFile.samplerate();

	// calculate frequency
	frequency = FREQ / sampleRate;
    
	// read data
    buffer = new int[sampleRate];
    dataLength = inputFile.read(buffer, sampleRate);

	// check if data are present
	if (dataLength <= 1) {
		std::cerr << "Nepodarilo se nacist zadna data." << std::endl;
		delete[] buffer;
		return EXIT_FAILURE;
	}
    
	// get baud size
	if ((baudSize = getBaudSize(buffer, dataLength, frequency)) <= 0) {
		delete[] buffer;
		return EXIT_FAILURE;
	}

	// demodulate
	if (demodulate(&inputFile, buffer, dataLength, frequency, baudSize, &output) < 0) {
		delete[] buffer;
		return EXIT_FAILURE;
	}

	// deleteb uffer
	delete[] buffer;

	// open output file stream
	std::ofstream out(filename, std::fstream::out | std::fstream::binary);

	if (! out.is_open()) {
		std::cerr << "Nepodarilo se otevrit vystupni soubor: " + filename << std::endl;
		return EXIT_FAILURE;
	}

	// write data into file
	out << output << std::endl;

	// close output file
	out.close();
    
    return EXIT_SUCCESS;
}


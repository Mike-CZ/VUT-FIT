/*
* Soubor:  FileLoader.h
* Datum:   2015/12/05
* Autori:  Lukas Pelanek, xpelan03@stud.fit.vutbr.cz
*		   Jiri Komarek, xkomar27@stud.fit.vutbr.cz
* Projekt: Simulator cislicovych obvodu, projekt do predmetu IMS
* Popis:   Program nacte zdrojovou topologii v NETlistu a vygeneruje script pro gnuplot pomoci ktereho se vykresli casovy prubeh
*/

#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>

#include "Wire.h"
#include "Gate.h"
#include "Calendar.h"

#define DATA_FILE "data.dat"
#define SCRIPT_FILE "script.gp"

// frekvence vzorkovani (jak casto se provede vypis hodnot) v nanosekundach
#define SAMPLE_FREQUENCY 10

/*
* Struktura uchovava predchozi hodnoty pri vypisu
*/
typedef struct oldStr {
	Gate* gate;
	int value;
}tOldVals;

int getComponents(const std::string fileName, std::vector<Gate*> *gates, std::vector<Wire*> *wires);
void clearComponents(std::vector<Gate*> &gates, std::vector<Wire*> &wires);
std::string Sample(std::vector<Gate*> &gates, std::vector<tOldVals> &oldVals);
std::vector<tOldVals> getInitValues(std::vector<Gate*> &gates);
int createConfigFile(std::vector<Gate*> &gates, char* name);
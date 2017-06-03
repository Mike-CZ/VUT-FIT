/*
* Soubor:  Main.cpp
* Datum:   2015/12/05
* Autori:  Lukas Pelanek, xpelan03@stud.fit.vutbr.cz
*		   Jiri Komarek, xkomar27@stud.fit.vutbr.cz	   
* Projekt: Simulator cislicovych obvodu, projekt do predmetu IMS
* Popis:   Program nacte zdrojovou topologii v NETlistu a vygeneruje script pro gnuplot pomoci ktereho se vykresli casovy prubeh
*/

#include <iostream>
#include <vector>
#include <sstream>
#include <string>

#include "FileLoader.h"
#include "Wire.h"
#include "Gate.h"
#include "Calendar.h"

// doba simulace
#define RUN_TIME 500 


// inicializace pocatecnich hodnot
double StartTime_ = 0;
double EndTime_ = RUN_TIME;

double Time_ = 0;

// konstantni ukazatele
const double &StartTime = StartTime_;
const double &Time = Time_;
const double &EndTime = EndTime_;

int main(int argc, char **argv) {

	std::vector<Gate*> gates;
	std::vector<Wire*> wires;
	std::vector<tOldVals> values;
	std::string fileName;
	std::string buffer;
	std::ofstream outFile;

	// kontrola zadani vstupniho souboru
	if (argc != 3) {
		std::cerr << "Spatne zadane vstupni parametry, navod na spusteni:" << std::endl;
		std::cerr << "\t Aplikaci je potreba spustit se dvema parametry" << std::endl;
		std::cerr << "\t 1. parametr je nazev souboru, ktery obsahuje vstupni topologii" << std::endl;
		std::cerr << "\t 2. parametr je nazev obrazku, ktery se ma vygenerovat (format png)" << std::endl;
		std::cerr << "\t po spusteni se vygeneruji dva soubory, \"data.dat\" a \"script.gp\"" << std::endl;
		std::cerr << "\t vysledny soubor \"script.gp\" spustte pomoci gnuplot" << std::endl;
		std::cerr.flush();
		return 1;
	}
	else {
		fileName = argv[1];
	}

	Calendar* calendar = Calendar::getInstance();

	// nacti komponenty
	if (getComponents(fileName, &gates, &wires) != 0) {
		clearComponents(gates, wires);
		calendar->Destroy();
		return 1;
	}

	// propoj hradla a draty
	Gate::ConnectWiresAndGates(gates, wires);

	Process* proc = calendar->getFirst();
	
	// hlavni smycka
	while (Time <= EndTime) {
		proc = (proc == NULL) ? calendar->getFirst() : proc;

		// pokud nadesel cas procesu, spustime jej
		if (proc != NULL && proc->getTime() <= Time) {
			calendar->runProcess(proc);
			proc = NULL;
		}

		// vypis hodnot na hradlech	
		if ((proc != NULL && proc->getTime() > Time) || (proc == NULL && calendar->isEmpty())) {
			if ((int)Time % SAMPLE_FREQUENCY == 0) {
				buffer += Sample(gates, values);
			}
			Time_ += 1.0;
			continue;
		}

		// jestlize pristi udalost je naplanovana az po case simulace, skoncime
		if (proc != NULL && proc->getTime() > EndTime) {
			break;
		}

	} // konec smycky
	
	outFile.open(DATA_FILE);
	if (!outFile) {
		std::cerr << "Nepodarilo se ulozit vysledek do souboru" << std::endl;
		std::cerr.flush();
		calendar->Destroy();
		clearComponents(gates, wires);
		return 1;
	}
	outFile << buffer;
	outFile.close();

	if (createConfigFile(gates, argv[2]) != 0) {
		std::cerr << "Nepodarilo se vygenerovat vystupni script" << std::endl;
		std::cerr.flush();
		calendar->Destroy();
		clearComponents(gates, wires);
		return 1;
	}

	calendar->Destroy();
	clearComponents(gates, wires);
	
	return 0;
}
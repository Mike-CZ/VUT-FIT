/*
* Soubor:  Wire.cpp
* Datum:   2015/12/05
* Autori:  Lukas Pelanek, xpelan03@stud.fit.vutbr.cz
*		   Jiri Komarek, xkomar27@stud.fit.vutbr.cz
* Projekt: Simulator cislicovych obvodu, projekt do predmetu IMS
* Popis:   Program nacte zdrojovou topologii v NETlistu a vygeneruje script pro gnuplot pomoci ktereho se vykresli casovy prubeh
*/

#include "Wire.h"

/*
* Konstruktor, nastaveni pocatecnich hodnot a prirazeni indexu dratu
*/
Wire::Wire(int index) {
	index_ = index;
	value_ = V_UNDEFINED;
	inConnected_ = false;
	outConnected_ = false;
}

/*
* Funkce vlozi novy drat do vectoru, pokud neexistuje
*/
void Wire::AddWire(std::vector<Wire*> &wires, int index) {
	for (std::vector<Wire*>::size_type i = 0; i != wires.size(); ++i) {
		if (wires[i]->getIndex() == index) {
			return;
		}
	}
	wires.push_back(new Wire(index));
}

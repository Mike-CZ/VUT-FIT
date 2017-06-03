/*
* Soubor:  FileLoader.cpp
* Datum:   2015/12/05
* Autori:  Lukas Pelanek, xpelan03@stud.fit.vutbr.cz
*		   Jiri Komarek, xkomar27@stud.fit.vutbr.cz
* Projekt: Simulator cislicovych obvodu, projekt do predmetu IMS
* Popis:   Program nacte zdrojovou topologii v NETlistu a vygeneruje script pro gnuplot pomoci ktereho se vykresli casovy prubeh
*/

#include "FileLoader.h"

/*
* Funkce projde zdrojovy soubor a pripravi komponenty
*/
int getComponents(const std::string fileName, std::vector<Gate*> *gates, std::vector<Wire*> *wires) {

	std::ifstream file(fileName);
	std::string line;
	int fileLine = 0;

	if (!file) {
		std::cerr << "Chyba pri otevirani souboru!" << std::endl;
		return 1;
	}
	try {
		while (std::getline(file, line))
		{
			++fileLine;
			// preskoc prazdne radky
			if (line.length() == 0) {
				continue;
			}

			std::istringstream ss(line);
			std::string component;
			std::string test;
			int in1 = 0;
			int in2 = 0;
			int out = 0;

			// uloz hodnoty
			ss >> component >> in1 >> in2 >> out >> test;

			// test formatu souboru
			if (test.length() != 0) {
				throw 1;
			}

			if (component == "IN" || component == "CLK") {
				if (in2 != 0 && out != 0) {
					throw 4;
				}
				else if (in1 < 1) {
					throw 3;
				}
			}
			else if (component == "NOT") {
				if (out != 0) {
					throw 4;
				}
				else if (in2 < 0) {
					throw 4;
				}
				else if (in1 < 1) {
					throw 3;
				}
			}
			else {
				if (in1 < 1 || in2 < 1) {
					throw 3;
				}
				else if (out < 0) {
					throw 4;
				}
			}

			if (component == "AND") {
				gates->push_back(new AndGate(in1, in2, out));
			}
			else if (component == "OR") {
				gates->push_back(new OrGate(in1, in2, out));
			}
			else if (component == "NOT") {
				gates->push_back(new NotGate(in1, in2));
			}
			else if (component == "NAND") {
				gates->push_back(new NandGate(in1, in2, out));
			}
			else if (component == "NOR") {
				gates->push_back(new NorGate(in1, in2, out));
			}
			else if (component == "IN") {
				gates->push_back(new Input(in1));
			}
			else if (component == "CLK") {
				gates->push_back(new Clk(in1));
			}
			else {
				throw 2;
			}

			// vlozeni dratu
			if (in1 > 0) {
				Wire::AddWire(*wires, in1);
			}
			if (in2 > 0) {
				Wire::AddWire(*wires, in2);
			}
			if (out > 0) {
				Wire::AddWire(*wires, out);
			}

		}
	}
	catch (int err) {
		if (err == 1) {
			std::cerr << "Chybny format souboru!" << std::endl;
		}
		else if (err == 2) {
			std::cerr << "Neznamy typ hradla na radku: " << fileLine << std::endl;
		}
		else if (err == 3) {
			std::cerr << "Chyba pri zadavani vstupniho pinu na radku: " << fileLine << " (Vstupni piny musi byt vetsi nez 0)" << std::endl;
		}
		else if (err == 4) {
			std::cerr << "Chyba pri zadavani vystupniho pinu na radku: " << fileLine << " (Vystupni pin nesmi byt mensi nez 0)" << std::endl;
		}
		file.close();
		return 1;
	}
	return 0;
}

/*
* Funkce znici vsechny komponenty a vyprazdni vector
*/
void clearComponents(std::vector<Gate*> &gates, std::vector<Wire*> &wires) {

	for (std::vector<Gate*>::size_type i = 0; i != gates.size(); ++i) {
		delete gates[i];
	}
	gates.clear();

	for (std::vector<Gate*>::size_type i = 0; i != wires.size(); ++i) {
		delete wires[i];
	}
	wires.clear();
}

/*
* Naplni pocatecni vektor uchovavajici predchozi hodnoty
*/
std::vector<tOldVals> getInitValues(std::vector<Gate*> &gates) {
	std::vector<tOldVals> vals;
	tOldVals tmp;

	for (std::vector<Gate*>::size_type i = 0; i != gates.size(); i++) {
		tmp.gate = gates[i];
		tmp.value = gates[i]->getIntValue() + (i * 3);
		vals.push_back(tmp);
	}

	return vals;
}

/*
* Vytiskne naformatovane hodnoty hradel na stdout
*/
std::string Sample(std::vector<Gate*> &gates, std::vector<tOldVals> &oldVals) {
	std::string values;
	std::string previousValues;
	bool valuesChanged = false;
	std::string buffer;

	for (std::vector<Gate*>::size_type i = 0; i != gates.size(); i++) {
		values += std::to_string(gates[i]->getIntValue() + (i * 3));
		values += " ";
		for (std::vector<tOldVals>::size_type x = 0; x != oldVals.size(); x++) {
			if (gates[i] == oldVals[x].gate) {

				previousValues += std::to_string(oldVals[x].value);
				previousValues += " ";

				// pokud se zmenila nejaka hodnota, vypiseme v tomto bode obe hodnoty, abychom dosahli plynuleho prechodu
				if ((int)(gates[i]->getIntValue() + (i * 3)) != oldVals[x].value) {
					valuesChanged = true;
				}
			}
		}
	}
	// ulozime predchozi hodnoty
	oldVals = getInitValues(gates);

	if (valuesChanged) {
		buffer += std::to_string((int)Time) + " " + previousValues + "\n";
	}
	buffer += std::to_string((int)Time) + " " + values + "\n";

	return buffer;
}

/*
* Vytvori script pro GNUplot a ulozi do souboru SCRIPT_FILE
*/
int createConfigFile(std::vector<Gate*> &gates, char *name) {
	std::ofstream outFile;
	outFile.open(SCRIPT_FILE);
	if (!outFile) {
		return 1;
	}
	int line = 0;
	std::string imageName(name);

	std::string content;
	
	content = std::string("set terminal png size 1200,600\n") +
		"set output '" + imageName + "'\n" +
		"set xtics font \"Arial, 10\"\n" +
		"set ytics font \"Arial, 9\"\n" +
		"set y2tics font \"Arial, 10\"\n" +
		"set xtics " + std::to_string(SAMPLE_FREQUENCY) + "\n" +
		"set xtics rotate\n" +
		"set xrange [0:]\n" +
		"set yrange [0:]\n" +
		"set format y \"\"\n" +
		"set format y2 \"\"\n" +
		"set xlabel 'Cas (ns)'\n" +
		"set ylabel 'Hodnota'\n" +
		"set grid\n" +
		"set grid xtics xtics\n" +
		"set grid ytics ytics\n" +
		"unset key\n" +
		"set style line 1 linecolor rgb \"blue\"\n" +
		"set xtics rotate\n"
		"set border 10\n";

	// vytvori prislusne nazvy pro hodnoty na ose Y
	for (std::vector<Gate*>::size_type x = 0; x < gates.size(); ++x) {
		for (int y = 0; y < 3; ++y) {
			switch (line % 3) {
			case 0:
				content += "set ytics add (\"X\" " + std::to_string(line) + ")\n";
				++line;
				break;
			case 1:
				content += "set ytics add (\"0\" " + std::to_string(line) + ")\n";
				content += "set y2tics add (\"" + gates[x]->getComponentInto() + "\" " + std::to_string(line) + ")\n";
				++line;
				break;
			case 2:
				content += "set ytics add (\"1\" " + std::to_string(line) + ")\n";
				++line;
				break;
			}
		}
	}

	// spojeni sloupcu
	content += "plot '" DATA_FILE "' using 1:2 with lines ls 1";

	for (std::vector<Gate*>::size_type i = 3; i <= (gates.size() + 1); ++i) {
		content += ", '" DATA_FILE "' using 1:" + std::to_string(i) + " with lines ls 1";
	}
	content += "\n";

	outFile << content;
	outFile.close();
	return 0;
}
/*
* Soubor:  Chooser.cpp
* Datum:   2017/4/20
* Autor:   Bc. Lukas Pelanek, xpelan03@stud.fit.vutbr.cz
* Projekt: PDS, Man in the Middle
* Popis:   Aplikace pro interaktivni vyber dvojic obeti.
*/


#include "Chooser.h"


std::vector<I_node> devices;
std::vector<unsigned int> indexes;
std::ifstream input;
std::ofstream output;
t_scannerParams params;

/* Signal handler */
void signalHandler(int signum) {
	// zapis do souboru
	output.open(params.file);
	Chooser::printXML(&output, &devices, &indexes);
	std::cout << std::endl << "Ukonceni aplikace. Tisk XML souboru." << std::endl;
	exit(signum);
}

int main(int argc, char **argv) {
	// registrace signal handleru
	signal(SIGINT, signalHandler);


	int errCode;

	// nacteni vstupnich parametru
	if ((errCode = IOHandler::getChooserParameters(argc, argv, &params)) != IOHandler::ERR_NO_ERROR) {
		IOHandler::printError(errCode);
		return -1;
	}

	// otevreni vstupniho souboru (parametry nacteny jako scanner params... interface == input)
	input.open(params.interface, std::ifstream::in);

	Chooser program(&input, &output, &devices, &indexes);

	if ((errCode = program.start()) != IOHandler::ERR_NO_ERROR) {
		IOHandler::printError(errCode);
		return -1;
	}

	// zapis do souboru
	output.open(params.file);
	if ((errCode = Chooser::printXML(&output, &devices, &indexes)) != IOHandler::ERR_NO_ERROR) {
		IOHandler::printError(errCode);
		return -1;
	}
	
	return 0;
}

/* Zkontroluje, zda index se nachazi ve vectoru */
bool Chooser::_isChosen(unsigned int index, std::vector<unsigned int> *indexes) {
	unsigned int x;
	for (x = 0; x < indexes->size(); x++) {
		if (indexes->at(x) == index) {
			return true;
		}
	}
	return false;
}

/* Tisk XML souboru */
int Chooser::printXML(std::ofstream *output, std::vector<I_node> *devices, std::vector<unsigned int> *indexes) {
		unsigned int x, y;
		int index = 1;

		if (!output->is_open()) {
			return IOHandler::ERR_OUT_FILE;
		}

		*output << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl;
		*output << "<devices>" << std::endl;

		// tisk sparovanych zarizeni
		for (y = 0; y < indexes->size(); y++) {
			*output << "\t<host mac=\"" << devices->at(indexes->at(y)).mac;
			*output << "\" group=\"victim-pair-" << index;
			*output << "\">" << std::endl;
			if ((y + 1) % 2 == 0) {
				index++;
			}

			for (x = 0; x < devices->at(indexes->at(y)).ipv4Addresses.size(); x++) {
				*output << "\t\t<ipv4>" << devices->at(indexes->at(y)).ipv4Addresses[x] << "</ipv4>" << std::endl;
			}

			for (x = 0; x < devices->at(indexes->at(y)).ipv6Addresses.size(); x++) {
				*output << "\t\t<ipv6>" << devices->at(indexes->at(y)).ipv6Addresses[x] << "</ipv6>" << std::endl;
			}
			*output << "\t</host>" << std::endl;
		}

		for (y = 0; y < devices->size(); y++) {
			if (!_isChosen(y, indexes)) {
				*output << "\t<host mac=\"" << devices->at(y).mac;
				*output << "\">" << std::endl;

				for (x = 0; x < devices->at(y).ipv4Addresses.size(); x++) {
					*output << "\t\t<ipv4>" << devices->at(y).ipv4Addresses[x] << "</ipv4>" << std::endl;
				}

				for (x = 0; x < devices->at(y).ipv6Addresses.size(); x++) {
					*output << "\t\t<ipv6>" << devices->at(y).ipv6Addresses[x] << "</ipv6>" << std::endl;
				}
				*output << "\t</host>" << std::endl;
			}
		}

		*output << "</devices>" << std::endl;

		IOHandler::closeOutputFile(output);

		return 0;
}

/* Konstruktor */
Chooser::Chooser(std::ifstream *input, std::ofstream *output, std::vector<I_node> *devices, std::vector<unsigned int> *indexes) {
	_input = input;
	_output = output;
	_devices = devices;
	_indexes = indexes;
}

 /* Start aplikace */
int Chooser::start() {
	unsigned int x, y;
	int index1, index2;
	std::string line;
	bool print;
	int errCode = 0;
	std::vector<unsigned int> indexes;

	if ((errCode = _init()) != IOHandler::ERR_NO_ERROR) {
		return errCode;
	}

	// napoveda
	std::cout << std::endl;
	std::cout << "*******************************************************" << std::endl;
	std::cout << "** Vyberte dvojici zarizeni ze seznamu pro sparovani **" << std::endl;
	std::cout << "** Indexy zarizeni napiste do konzole oddelene       **" << std::endl;
	std::cout << "** mezerou. Priklad: 0 1                             **" << std::endl;
	std::cout << "** Pro ukonceni stisknete ctrl+c nebo napiste end    **" << std::endl;
	std::cout << "*******************************************************" << std::endl;
	std::cout << std::endl;

	/* Nekonecna smycka obsluhujici aplikaci */
	while (true) {
		if (_list.size() == 0) {
			std::cout << "Zadna zarizeni ke sparovani!" << std::endl;
			return 0;
		}

		if (_indexes->size() == _list.size()) {
			std::cout << "Vsechna zarizeni byla sparovana. Generuji XML soubor." << std::endl;
			return 0;
		}

		for (x = 0; x < _list.size(); x++) {
			print = true;
			for (y = 0; y < _indexes->size(); y++) {
				if (_indexes->at(y) == x) {
					print = false;
					break;
				}
			}
			if (print) {
				std::cout << "Index: " << x << "\tMac adresa: " << _list[x].mac << std::endl;
			}
		}
		std::cout << "Zadejte indexy: ";
		std::getline(std::cin, line);

		if (line.length() != 3) {
			std::cout << "Neplatny vstup!" << std::endl;
			continue;
		}

		if (line == "end") {
			std::cout << "Aplikace ukoncena. Generuji XML soubor." << std::endl;
			return 0;
		}

		// nacteni hodnot
		std::istringstream inputStream(line);
		inputStream >> index1;
		inputStream >> index2;

		// osetreni indexu
		if ((index1 < 0 || index1 >= (int)_list.size()) || (index2 < 0 || index2 >= (int)_list.size())) {
			std::cout << "Zadane cislo neodpovida zadnemu indexu!" << std::endl;
			continue;
		}
		else if (index1 == index2) {
			std::cout << "Indexy musi byt od sebe ruzne!" << std::endl;
			continue;
		}

		print = true;

		for (x = 0; x < _indexes->size(); x++) {
			if ((int)_indexes->at(x) == index1) {
				std::cout << "Zadany index byl jiz pouzit!" << std::endl;
				print = false;
				break;
			}
			if ((int)_indexes->at(x) == index2) {
				std::cout << "Zadany index byl jiz pouzit!" << std::endl;
				print = false;
				break;
			}
		}

		// vlozeni indexu
		if (print) {
			_indexes->push_back(index1);
			_indexes->push_back(index2);
		}
	}

	return 0;
}

/* Inicializace */
int Chooser::_init() {

	unsigned int x, y, z, counter;
	std::size_t position;
	std::string token;
	I_node node;

	enum L_ERROR {
		L_START,
		L_END,
		L_HOST,
		L_ADDR,
		L_IPV4,
		L_IPV6,
		L_CHECK
	};

	if (!_input->is_open()) {
		return IOHandler::ERR_IN_FILE;
	}

	// nacteni vstupu do stringu
	std::string xml((std::istreambuf_iterator<char>(*_input)), std::istreambuf_iterator<char>());

	// zavri input
	_input->close();

	L_ERROR state = L_START;
	std::string tmp;

	// prvni token s xml hlavickou preskocime
	if (_readToken(&xml).size() <= 0) {
		return IOHandler::ERR_IN_FILE;
	}

	while ((token = _readToken(&xml)).length() > 0 && (state != L_END)) {
		switch (state) {
		case L_START:
			if (_toLower(token) != "<devices>") {
				return IOHandler::ERR_IN_FILE;
			}
			state = L_HOST;
			break;
		case L_HOST:
			if ((_toLower(token).substr(0, 5) != "<host" || _toLower(token).find("mac=\"") == std::string::npos) && _toLower(token) != "</devices>") {
				return IOHandler::ERR_IN_FILE;
			}
			if (_toLower(token) == "</devices>") {
				state = L_END;
				break;
			}

			// ziskani mac adresy
			tmp = token.substr(_toLower(token).find("mac=\"") + 5);
			if ((position = tmp.find("\"")) == std::string::npos) {
				return IOHandler::ERR_IN_FILE;
			}
			tmp = tmp.substr(0, position);
			if (tmp.size() != 14) {
				return IOHandler::ERR_IN_FILE;
			}
			node.mac = _toLower(tmp);

			state = L_ADDR;
			break;
		case L_ADDR:
			// nacteneho hosta vlozim do seznamu a vyprazdnim docasny uzel
			if (_toLower(token) == "</host>") {
				if (node.mac.length() > 0 && (node.ipv4Addresses.size() > 0 || node.ipv6Addresses.size() > 0)) {
					_list.push_back(node);
					_devices->push_back(node);
					node.mac.clear();
					node.ipv4Addresses.clear();
					node.ipv6Addresses.clear();
				}
				state = L_HOST;
				break;
			}
			if (_toLower(token) != "<ipv4>" && _toLower(token) != "<ipv6>") {
				return IOHandler::ERR_IN_FILE;
			}

			if (_toLower(token) == "<ipv4>") {
				if ((position = xml.find("<")) == std::string::npos) {
					return IOHandler::ERR_IN_FILE;
				}
				tmp = _removeSpaces(xml.substr(0, position));
				node.ipv4Addresses.push_back(tmp);
			}
			else if (_toLower(token) == "<ipv6>") {
				if ((position = xml.find("<")) == std::string::npos) {
					return IOHandler::ERR_IN_FILE;
				}
				tmp = _removeSpaces(xml.substr(0, position));
				node.ipv6Addresses.push_back(tmp);
			}
			state = L_CHECK;
			break;
		case L_CHECK:
			if (_toLower(token) != "</ipv4>" && _toLower(token) != "</ipv6>") {
				return IOHandler::ERR_IN_FILE;
			}
			state = L_ADDR;
			break;
		default:
			break;
		}
	}
	if (state != L_END) {
		return IOHandler::ERR_IN_FILE;
	}

	return 0;
}

/* Odstrani mezery z retezce */
std::string Chooser::_removeSpaces(std::string in) {
	in.erase(std::remove(in.begin(), in.end(), ' '), in.end());
	return in;
}

/* Prevede string na lower case */
std::string Chooser::_toLower(std::string word) {
	std::transform(word.begin(), word.end(), word.begin(), ::tolower);
	return word;
}

/* nacte token */
std::string Chooser::_readToken(std::string *inString) {
	std::size_t pos1;
	std::size_t pos2;
	std::string str;

	if ((pos1 = inString->find("<")) == std::string::npos) {
		return str;
	}
	if ((pos2 = inString->find(">")) == std::string::npos) {
		return str;
	}

	str = inString->substr(pos1, pos2 - pos1 + 1);

	*inString = inString->substr(pos2 + 1);

	return str;
}
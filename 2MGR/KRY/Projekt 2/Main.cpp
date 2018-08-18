/*
* Soubor:  Main.cpp
* Datum:   2018//4/28
* Autor:   Bc. Lukas Pelanek, xpelan03@stud.fit.vutbr.cz
* Projekt: Kryptografie, proj 2
* Popis:   Generovani klicu RSA, sifrovani, desifrovani, prolomeni sifry
*/

#include <iostream>
#include "RSA.h"

int main(int argc, char **argv)
{
	std::string result;

	// parse args and call required functionality
	if (argc == 2 && std::string(argv[1]) == "-h") {
		RSA::printHelp();
		return 0;
	}
	else if (argc == 3 && std::string(argv[1]) == "-g") {
		result = RSA::generateKeys(std::string(argv[2]));
	}
	else if (argc == 3 && std::string(argv[1]) == "-b") {
		result = RSA::breakCypher(std::string(argv[2]));
	}
	else if (argc == 5 && std::string(argv[1]) == "-e") {
		result = RSA::cryptMessage(std::string(argv[2]), std::string(argv[3]), std::string(argv[4]));
	}
	else if (argc == 5 && std::string(argv[1]) == "-d") {
		result = RSA::cryptMessage(std::string(argv[2]), std::string(argv[3]), std::string(argv[4]));
	}
	else {
		std::cout << "Chyba pri zadavani parametru. Spustte aplikaci s prepinacem -h pro vypsani napovedy" << std::endl;
		return 1;
	}

	// return error code if result empty
	if (result.empty()) {
		return 1;
	}

	// print result
	std::cout << result << std::endl;

	return 0;
}
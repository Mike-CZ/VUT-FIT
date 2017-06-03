/*
* Soubor:  Chooser.h
* Datum:   2017/4/20
* Autor:   Bc. Lukas Pelanek, xpelan03@stud.fit.vutbr.cz
* Projekt: PDS, Man in the Middle
* Popis:   Aplikace pro interaktivni vyber dvojic obeti.
*/

#ifndef CHOOSER_H
#define CHOOSER_H

#include <csignal>
#include <cstring>
#include <algorithm>
#include <sstream>
#include <string> 
#include <cstdlib>
#include <iostream>
#include <vector>

#include "IOHandler.h"


typedef struct {
	std::string mac;
	std::vector<std::string> ipv4Addresses;
	std::vector<std::string> ipv6Addresses;
} I_node;

void signalHandler(int signum);

class Chooser {
private:
	std::ifstream *_input;
	std::ofstream *_output;
	std::vector<I_node> *_devices;
	std::vector<unsigned int> *_indexes;
	std::vector<I_node> _list;

	int _init();
	std::string _toLower(std::string word);
	std::string _removeSpaces(std::string in);
	std::string _readToken(std::string *inString);
	bool static _isChosen(unsigned int index, std::vector<unsigned int> *indexes);

public:
	Chooser(std::ifstream *input, std::ofstream *output, std::vector<I_node> *devices, std::vector<unsigned int> *indexes);
	static int printXML(std::ofstream *output, std::vector<I_node> *devices, std::vector<unsigned int> *indexes);
	int start();
};


#endif

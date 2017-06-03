/**
* ISA project
* Name: Klient SIP
* Author: Lukas Pelanek, xpelan03@stud.fit.vutbr.cz, 2015
**/

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cstring>

#include "Message.h"

using namespace std;

enum Errors{
	OK = 0,
	PARAMSERR,
	FILEERR,
	FOPENERR,
	SERR,
	INITERR,
	RERR,
	BINDERR,
	IPERR
};

const string MESSAGES[] = {
	"OK",
	"Chyba pri zpracovani parametru",
	"Chyba pri zpracovani souboru",
	"Nepodarilo se otevrit soubor",
	"Nepodarilo se otevrit socket",
	"Chyba pri inicializaci",
	"Chyba pri cteni ze socketu",
	"Chyba pri bindovani portu, port je obsazen",
	"Chyba pri zjistovani IP adresy stroje"
};

typedef struct Profile {
	string server;
	string port;
	string userName;
	string password;
	int expire;
}tProfile;

typedef struct responseStatus {
	string branch;
	string identifier;
}respStat;

inline bool isInteger(const std::string & s);
Errors getParams(tProfile *profile, vector<Message*> *messages, int argc, char **argv);
Errors getMessages(vector<Message*> *messages, char *path);
Errors getProfile(tProfile *profile, char *path);

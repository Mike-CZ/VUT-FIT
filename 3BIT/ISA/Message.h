/**
* ISA project
* Name: Klient SIP
* Author: Lukas Pelanek, xpelan03@stud.fit.vutbr.cz, 2015
**/

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

class Message {
private:
	string ipAddress;
	string rawIP;
	string message;
	string port;

public:
	Message(string ip, string port, string msg, string raw){
		this->ipAddress = ip;
		this->message = msg;
		this->port = port;
		this->rawIP = raw;
	}
	string getIP();
	string getRawIP();
	string getMessage();
	string getPort();
};

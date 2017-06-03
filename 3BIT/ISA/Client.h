/**
* ISA project
* Name: Klient SIP
* Author: Lukas Pelanek, xpelan03@stud.fit.vutbr.cz, 2015
**/

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <cctype>
#include <fcntl.h>
#include <sstream>
#include <string>
#include "Parameters.h"
#include "Message.h"
#include <time.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <openssl/md5.h>
#include <sys/types.h>
#include <ifaddrs.h>

using namespace std;

#define PORT 5061
#define BUFFER 1024
#define ATTEMPT_TIME 5
#define UNREGISTER_TIME 1

typedef struct responseValues{
	string from;
	string to;
	string realm;
	string nonce;
	string callID;
	string fromTag;
	string toTag;
	string branch;
	string returnIP;
	string returnPort;
	string cSeq;
	string expires;
	string contentLength;
}respVal;

string intToString(int number);
string myToUpper(string lowerCase);
string hashMD5(string str);
string randomString(const int len);
string printPort(string port);
bool isNumber(string val);
string removeAndPrint(string address);

// singleton pattern
class Client {
private:
	string clientIP;
	string clientPort;
	string suffix;
	respVal registerParams;
	string registerHash;
	unsigned int cSeq;
	struct sockaddr_in client;
	struct sockaddr_in address;
	tProfile profile;
	vector<Message*> messages;
	vector<respStat> msgstats;
	string proceedResponse(char *buffer);
	string getClientRegister();
	string getClientRegisterAuthorize(responseValues vals);
	string getMessage(Message *msg);
	string getMMessageReply(responseValues vals);
	string getMMessageErrReply(responseValues vals);
	void printSentMessage(string number, string request, string from, string to, string message);
	Errors init();
	string logoutMessage;
	bool sendMessages;
	bool TrynextLoginAttempt;
	bool nextIsAuthentication;
	bool nextIsUnregister;

	bool getFinalMessage;
	int nextRegisterIn;
	
	static Client *instance;
	static bool instanceFlag;
	Client() {
		this->cSeq = 1;
		this->sNumber = 1;
		this->clientIP = "";
		this->clientPort = "5060";
		this->logoutMessage = "";
		this->registerHash = "";
		this->isRegistered = false;
		this->getMessages = true;
		this->sendMessages = false;
		this->TrynextLoginAttempt = false;
		this->nextIsAuthentication = false;
		this->nextIsUnregister = false;
		this->getFinalMessage = false;
		this->nextRegisterIn = 0;
	}

public:
	~Client() {
		instanceFlag = false;
	}
	static Client* getInstance();
	bool isRegistered;
	int clientSocket;
	struct sockaddr_in server;
	string getClientUnregister();

	bool getMessages;
	int sNumber;
	void setParams(tProfile p, vector<Message*> m) {
		this->profile = p;
		this->messages = m;
		
	}
	Errors run();
};

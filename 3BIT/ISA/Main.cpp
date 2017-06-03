/**
* ISA project
* Name: Klient SIP
* Author: Lukas Pelanek, xpelan03@stud.fit.vutbr.cz, 2015
**/

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <csignal>

#include "Parameters.h"
#include "Message.h"
#include "Client.h"

using namespace std;

void signalHandler(int signum) {
	Client *cl = Client::getInstance();
	int num = signum;
	// stop recieving messages, also check for UNREGISTER message
	cl->getMessages = false;

	if (cl->isRegistered) {
		socklen_t length = sizeof(cl->server);
		string msg;

		// get unregister message
		msg = cl->getClientUnregister();

		// send unregister message
		sendto(cl->clientSocket, msg.c_str(), msg.length(), 0, (struct sockaddr *) &cl->server, length);
		++cl->sNumber;

	}
}

int main(int argc, char** argv) {
	signal(SIGINT, signalHandler);
	signal(SIGTERM, signalHandler);
	signal(SIGQUIT, signalHandler);

	tProfile profile = { "", "", "", "", 0 };
	vector<Message*> messages;
	Errors err;

	if ( (err = getParams(&profile, &messages, argc, argv)) != OK){
		cerr << MESSAGES[err].c_str() << endl;
		cerr.flush();
		return 1;
	}

	Client *client = Client::getInstance();
	client->setParams(profile, messages);
	if ((err = client->run()) != OK) {
		cerr << MESSAGES[err].c_str() << endl;
		cerr.flush();
		return 1;
	}

	return 0;
}

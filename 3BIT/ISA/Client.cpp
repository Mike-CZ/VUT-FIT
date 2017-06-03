/**
* ISA project
* Name: Klient SIP
* Author: Lukas Pelanek, xpelan03@stud.fit.vutbr.cz, 2015
**/

#include "Client.h"


bool Client::instanceFlag = false;
Client* Client::instance = NULL;
Client* Client::getInstance() {
	if (!instanceFlag)
	{
		instance = new Client();
		srand(time(NULL));
		instance->cSeq = rand() % 100 + 1;
		instanceFlag = true;
		return instance;
	}
	else
	{
		return instance;
	}
}


/**
* Runs the client
* @return error code
*/
Errors Client::run() {
	socklen_t length = sizeof(this->server);
	char buf[BUFFER];
	string msg;

	if (this->init() != OK) {
		return SERR;
	}

	// send register packet
	msg = this->getClientRegister();
	sendto(this->clientSocket, msg.c_str(), msg.length(), 0, (struct sockaddr *) &this->server, length);

	// increase sequence number
	++this->sNumber;

	bool loop = true;
	int sentMessages = 0;
	int messagesToSend = this->messages.size();
	struct hostent *hostent;

	memset(&address, 0, sizeof(this->address));
	
	socklen_t adLen = sizeof(this->address);
	this->address.sin_family = AF_INET;

	bool newRegister = false;
	clock_t tStart = clock();
	clock_t registerTime = clock();
	bool finalCount = false;
	bool nextRegister = false;

	while (loop) {

		// try next registration if failed
		if (this->TrynextLoginAttempt) {
			tStart = clock();
			newRegister = true;
			this->TrynextLoginAttempt = false;
		}

		if (newRegister && !finalCount) {
			if (((clock() - tStart) / CLOCKS_PER_SEC) >= ATTEMPT_TIME) {
				newRegister = false;

				// send register packet
				msg = this->getClientRegister();
				sendto(this->clientSocket, msg.c_str(), msg.length(), 0, (struct sockaddr *) &this->server, length);

				// increase sequence number
				++this->sNumber;
			}
		}

		// send register packet in interval to keep client connected
		if (this->nextRegisterIn != 0 && this->isRegistered && !nextRegister) {
			nextRegister = true;
			registerTime = clock();
		}

		if (nextRegister && this->nextRegisterIn != 0) {
			if (((clock() - registerTime) / CLOCKS_PER_SEC) >= this->nextRegisterIn) {
				nextRegister = false;

				// send register packet
				msg = this->getClientRegister();
				sendto(this->clientSocket, msg.c_str(), msg.length(), 0, (struct sockaddr *) &this->server, length);

				// increase sequence number
				++this->sNumber;
			}
		}

		if (this->getMessages == false) {
			if (!finalCount) {
				tStart = clock();
				finalCount = true;
			}
		}

		// wait for response for defined time
		if (finalCount) {
			if (this->getFinalMessage || ! this->isRegistered) {
				loop = false;
				close(this->clientSocket);
				return OK;
			}
			if (((clock() - tStart) / CLOCKS_PER_SEC) >= UNREGISTER_TIME) {
				loop = false;
				close(this->clientSocket);
				return OK;
			}
		}

		// read from server
		if ( ! (recv(this->clientSocket, buf, BUFFER, 0) < 0)) {
			//cout << buf << endl;

			msg = this->proceedResponse(buf);
			if (msg.length() != 0) {
				sendto(this->clientSocket, msg.c_str(), msg.length(), 0, (struct sockaddr *) &this->server, length);
				// increase sequence number
				++this->sNumber;
			}
			//clear buffer
			memset(buf, '\0', BUFFER);
		}

		if ((sentMessages < messagesToSend) && this->isRegistered) {
			int clPort = 5060;
			if (this->messages[sentMessages]->getPort() != "" && isNumber(this->messages[sentMessages]->getPort())) {
				clPort = atoi(this->messages[sentMessages]->getPort().c_str());
			}
			this->address.sin_port = htons(clPort);

			if ((hostent = gethostbyname(this->messages[sentMessages]->getRawIP().c_str())) != NULL) {

				memcpy(&this->address.sin_addr, hostent->h_addr, hostent->h_length);
				msg = this->getMessage(this->messages[sentMessages]);
				sendto(this->clientSocket, msg.c_str(), msg.length(), 0, (struct sockaddr *) &this->address, adLen);
				// increase sequence number
				++this->sNumber;
			}
			++sentMessages;
		}

	}

	close(this->clientSocket);

	return OK;
}

string Client::getClientRegister() {
	string branch = "z9hG4bK" + randomString(8) + this->suffix;

	// TODO - profile.server <- jina hodnota
	string msg = string("REGISTER sip:") + this->profile.server + " SIP/2.0\r\n" +
		"Via: SIP/2.0/UDP " + this->clientIP + ":" + this->clientPort + ";branch=" + branch + ";rport\r\n" +
		"CSeq: " + intToString(this->cSeq) + " REGISTER\r\n" +
		"From: <sip:" + this->profile.userName + "@" + this->profile.server + printPort(this->profile.port) + ">;tag=" + randomString(8) + this->suffix + "\r\n" +
		"Call-ID: " + randomString(8) + this->suffix + "@" + this->profile.server + "\r\n" +
		"To: <sip:" + this->profile.userName + "@" + this->profile.server + printPort(this->profile.port) + ">\r\n" +
		"Contact: <sip:" + this->profile.userName + "@" + this->clientIP + ":" + this->clientPort + ">;q=1\r\n" +
		"Expires: " + intToString(this->profile.expire) + "\r\n" +
		"Content-Length: 0\r\n" +
		"Max-Forwards: 70\r\n\r\n";
	
	++this->cSeq;

	this->isRegistered = false;

	this->printSentMessage(intToString(this->sNumber), "REGISTER", this->profile.userName + "@" + this->profile.server + printPort(this->profile.port),
		this->profile.userName + "@" + this->profile.server + printPort(this->profile.port), "");

	// store branch and id to recognize response
	respStat stat;
	stat.branch = branch;
	stat.identifier = intToString(this->sNumber);
	this->msgstats.push_back(stat);

	return msg;
}

string Client::getMessage(Message *msg) {
	string branch = "z9hG4bK" + randomString(8) + this->suffix;

	string tmp = string("MESSAGE sip:") + msg->getIP() + ((msg->getPort() == "") ? "" : msg->getPort()) + " SIP/2.0\r\n" +
		"Via: SIP/2.0/UDP " + this->clientIP + ":" + this->clientPort + ";branch=" + branch + ";rport\r\n" +
		"CSeq: " + intToString(this->cSeq) + " MESSAGE\r\n" +
		"From: <sip:" + this->profile.userName + "@" + this->profile.server + printPort(this->profile.port) + ">\r\n" +
		"Call-ID: " + randomString(8) + this->suffix + "@" + this->profile.server + "\r\n" +
		"To: <sip:" + removeAndPrint(msg->getIP()) + ">\r\n" +
		"Expires: 5000\r\n" +
		"Content-Length: " + intToString(msg->getMessage().length()) + "\r\n" +
		"Content-Type: text/plain;charset=UTF-8\r\n" +
		"Max-Forwards: 70\r\n" +
		"\r\n" +
		msg->getMessage();

	++this->cSeq;

	this->printSentMessage(intToString(this->sNumber), "MESSAGE", this->profile.userName + "@" + this->profile.server + printPort(this->profile.port),
		removeAndPrint(msg->getIP()), msg->getMessage());

	// store branch and id to recognize response
	respStat stat;
	stat.branch = branch;
	stat.identifier = intToString(this->sNumber);
	this->msgstats.push_back(stat);

	return tmp;
}

string Client::getMMessageReply(responseValues vals) {
	struct hostent *hostent;
	socklen_t length = sizeof(this->address);

	string tmp = string("SIP/2.0 202 Accepted\r\n") +
		"Via: SIP/2.0/UDP " + vals.returnIP + ((vals.returnPort.length() == 0) ? "" : ":" + vals.returnPort) + ";branch=" + vals.branch + ";" +
		(vals.returnPort != "" ? "rport=" + vals.returnPort + ";" : "") + ";received=" + vals.returnIP + "\r\n" +
		"CSeq: " + vals.cSeq + " MESSAGE\r\n" +
		"From: <" + vals.from + ">\r\n" +
		"Call-ID: " + vals.callID + "\r\n" +
		"To: <" + vals.to + ">\r\n" +
		"Contact: <sip:" + this->profile.userName + "@" + this->clientIP + ":" + this->clientPort + ">\r\n" +
		"Content-Length: 0\r\n\r\n";

	// remove "sip:" prefix
	string upper = myToUpper(vals.from);
	string from = vals.from;
	string to = vals.to;
	if (!strncmp(upper.c_str(), "SIP:", 4)) {
		from = vals.from.substr(4);
	}
	upper = myToUpper(vals.to);
	if (!strncmp(upper.c_str(), "SIP:", 4)) {
		to = vals.to.substr(4);
	}

	this->printSentMessage(intToString(this->sNumber), "202 Accepted", this->profile.userName + "@" + this->profile.server + printPort(this->profile.port), from, "");

	if ((hostent = gethostbyname(vals.returnIP.c_str())) != NULL) {
		memcpy(&this->address.sin_addr, hostent->h_addr, hostent->h_length);
		this->address.sin_port = htons(atoi(vals.returnPort.c_str()));
		sendto(this->clientSocket, tmp.c_str(), tmp.length(), 0, (struct sockaddr *) &this->address, length);
	}
	++this->sNumber;

	return "";
}

string Client::getMMessageErrReply(responseValues vals) {
	struct hostent *hostent;
	socklen_t length = sizeof(this->address);

	string tmp = string("SIP/2.0 403 Forbidden\r\n") +
		"Via: SIP/2.0/UDP " + vals.returnIP + ((vals.returnPort.length() == 0) ? "" : ":" + vals.returnPort) + ";branch=" + vals.branch + ";" +
		(vals.returnPort != "" ? "rport=" + vals.returnPort + ";" : "") + ";received=" + vals.returnIP + "\r\n" +
		"CSeq: " + vals.cSeq + " MESSAGE\r\n" +
		"From: <sip:" + this->profile.userName + "@" + this->profile.server + printPort(this->profile.port) + ">\r\n" +
		"Call-ID: " + vals.callID + "\r\n" +
		"To: <" + vals.from + ">\r\n" +
		"Contact: <sip:" + this->profile.userName + "@" + this->clientIP + ":" + this->clientPort + ">\r\n" +
		"Content-Length: 0\r\n\r\n";

	// remove "sip:" prefix
	string upper = myToUpper(vals.from);
	string from = vals.from;
	string to = vals.to;
	if (!strncmp(upper.c_str(), "SIP:", 4)) {
		from = vals.from.substr(4);
	}
	upper = myToUpper(vals.to);
	if (!strncmp(upper.c_str(), "SIP:", 4)) {
		to = vals.to.substr(4);
	}

	this->printSentMessage(intToString(this->sNumber), "403 Forbidden", this->profile.userName + "@" + this->profile.server + printPort(this->profile.port), from, "");

	if ((hostent = gethostbyname(vals.returnIP.c_str())) != NULL) {
		memcpy(&this->address.sin_addr, hostent->h_addr, hostent->h_length);
		this->address.sin_port = htons(atoi(vals.returnPort.c_str()));
		sendto(this->clientSocket, tmp.c_str(), tmp.length(), 0, (struct sockaddr *) &this->address, length);
	}
	++this->sNumber;

	return "";
}

string Client::getClientRegisterAuthorize(responseValues vals) {

	string uri = "sip:" + this->profile.server;

	// get hash value
	string HA1 = this->profile.userName + ":" + vals.realm + ":" + this->profile.password;
	string HA2 = "REGISTER:" + uri;
	string tmp = hashMD5(HA1) + ":" + vals.nonce + ":" + hashMD5(HA2);
	string hash = hashMD5(tmp);
	string branch = "z9hG4bK" + randomString(8) + this->suffix;

	// TODO - profile.server <- jina hodnota
	string msg = string("REGISTER sip:") + this->profile.server + " SIP/2.0\r\n" +
		"Via: SIP/2.0/UDP " + this->clientIP + ":" + this->clientPort + ";branch=" + branch + ";rport\r\n" +
		"CSeq: " + intToString(this->cSeq) + " REGISTER\r\n" +
		"Authorization: Digest username=\"" + this->profile.userName + "\", realm=\"" + vals.realm + "\", nonce=\"" +
			vals.nonce + "\", uri=\"" + uri + "\", algorithm=MD5, response=\"" + hash + "\"\r\n" +
		"From: <sip:" + this->profile.userName + "@" + this->profile.server + printPort(this->profile.port) + ">;tag=" + vals.fromTag + "\r\n" +
		"Call-ID: " + vals.callID + "\r\n" +
		"To: <sip:" + this->profile.userName + "@" + this->profile.server + printPort(this->profile.port) + ">\r\n" +
		"Contact: <sip:" + this->profile.userName + "@" + this->clientIP + ":" + this->clientPort + ">;q=1\r\n" +
		"Expires: " + intToString(this->profile.expire) + "\r\n" +
		"Content-Length: 0\r\n" +
		"Max-Forwards: 70\r\n\r\n";

	// store current values for further unregister request
	this->registerParams = vals;
	this->registerHash = hash;

	++this->cSeq;

	this->nextIsAuthentication = true;

	this->printSentMessage(intToString(this->sNumber), ( ! this->isRegistered ? "REGISTER" : "UNREGISTER"), this->profile.userName + "@" + this->profile.server + printPort(this->profile.port),
		this->profile.userName + "@" + this->profile.server + printPort(this->profile.port), "");

	// store branch and id to recognize response
	respStat stat;
	stat.branch = branch;
	stat.identifier = intToString(this->sNumber);
	this->msgstats.push_back(stat);

	return msg;
}

string Client::getClientUnregister() {
	string branch = "z9hG4bK" + randomString(8) + this->suffix;
	string uri = "sip:" + this->profile.server;

	string msg = string("REGISTER sip:") + this->profile.server + " SIP/2.0\r\n" +
		"Via: SIP/2.0/UDP " + this->clientIP + ":" + this->clientPort + "; branch=" + branch + ";rport\r\n" +
		"CSeq: " + intToString(this->cSeq) + " REGISTER\r\n" +
		"Authorization: Digest username=\"" + this->profile.userName + "\", realm=\"" + this->registerParams.realm + "\", nonce=\"" +
					this->registerParams.nonce + "\", uri=\"" + uri + "\", algorithm=MD5, response=\"" + this->registerHash + "\"\r\n" +
		"From: <sip:" + this->profile.userName + "@" + this->profile.server + printPort(this->profile.port) + ">;tag=" + randomString(8) + this->suffix + "\r\n" +
		"Call-ID: " + this->registerParams.callID + "\r\n" +
		"To: <sip:" + this->profile.userName + "@" + this->profile.server + printPort(this->profile.port) + ">\r\n" +
		"Contact: <sip:" + this->profile.userName + "@" + this->clientIP + ":" + this->clientPort + ">\r\n" +
		"Expires: 0\r\n" +
		"Content-Length: 0\r\n" +
		"Max-Forwards: 70\r\n\r\n";

	// set expire value to 0
	this->profile.expire = 0;

	this->nextIsUnregister = true;

	++this->cSeq;

	this->printSentMessage(intToString(this->sNumber), "UNREGISTER", this->profile.userName + "@" + this->profile.server + printPort(this->profile.port),
		this->profile.userName + "@" + this->profile.server + printPort(this->profile.port), "");

	// store branch and id to recognize response
	respStat stat;
	stat.branch = branch;
	stat.identifier = intToString(this->sNumber);
	this->msgstats.push_back(stat);

	return msg;
}

string Client::proceedResponse(char *buffer){

	enum doAction {
		A_NONE = 0,
		A_REGISTER,
		A_MESSAGE,
		A_ERR_500,
		A_REGISTER_CONFIRMED
	};
	string wholeMessage = string(buffer);
	doAction action = A_NONE;
	string responseToServer = "";
	stringstream str;
	str << buffer;
	responseValues vals;
	string message = "";
	string line = "";
	string code = "";
	string codeMsg = "";
	string tmpMsg;
	string upperLine;
	bool firstLine = true;
	string::size_type position, splitSpace;
	bool isResponse = false;

	while (getline(str, line)) {
		// cast to upper case --> field names are case insensitive
		upperLine = myToUpper(line);

		// get response code
		if (firstLine){
			std::size_t found;

			// if SIP header is missing, ignore message
			if (upperLine.find("SIP") == std::string::npos) {
				return "";
			}

			if ((found = line.find_first_of(" ")) != std::string::npos) {
				tmpMsg = line.substr(found + 1);


				if ((found = tmpMsg.find_first_of(" ")) != std::string::npos) {
					string tmpCode = "";
					tmpCode = tmpMsg.substr(0, found);

					// response code
					if (tmpCode.length() == 3) {
						code = tmpCode;
						codeMsg = tmpMsg.substr(found + 1);
						if ((found = codeMsg.find_first_of("\r\n")) != std::string::npos) {
							codeMsg = codeMsg.substr(0, found);
						}
						// reply with register message
						if (code == "401") {
							action = A_REGISTER;
						}
						else if (!strncmp(code.c_str(), "5", 1)) {
							action = A_ERR_500;
						}
						else if (!strncmp(code.c_str(), "4", 1)) {
							if (this->nextIsAuthentication) {
								this->isRegistered = false;
							}
						}
						else if (!strncmp(code.c_str(), "2", 1)) {
							if (this->nextIsAuthentication && codeMsg.find("OK") != std::string::npos) {
								this->isRegistered = true;
								action = A_REGISTER_CONFIRMED;
							}
							if (this->nextIsUnregister && codeMsg.find("OK") != std::string::npos) {
								this->getFinalMessage = true;
							}
						}

						// if not message response
						if (strncmp(code.c_str(), "202", 3) != 0) {
							this->nextIsAuthentication = false;
						}
						isResponse = true;
					}
					// incoming message
					else if (!strncmp(upperLine.c_str(), "MESSAGE", 7)) {
						codeMsg = "MESSAGE";
						action = A_MESSAGE;
					}
					else {
						return "";
					}
				}
			}
			else {
				return "";
			}

			firstLine = false;
		}
		else{
			// parse message
			if (!strncmp(upperLine.c_str(), "FROM", 4)){
				position = line.find_first_of("<>;\n");
				tmpMsg = line.substr(position + 1);
				vals.from = tmpMsg.substr(0, tmpMsg.find_first_of("<>;\n"));

				std::size_t found = upperLine.find("TAG=");
				if (found != std::string::npos){
					tmpMsg = line.substr(found + 4);
					vals.fromTag = tmpMsg.substr(0, tmpMsg.find_first_of("\r\n;"));
				}
			}
			else if (!strncmp(upperLine.c_str(), "VIA", 3)){
				std::size_t found = upperLine.find("BRANCH=");
				if (found != std::string::npos){
					tmpMsg = line.substr(found + 7);
					vals.branch = tmpMsg.substr(0, tmpMsg.find_first_of("<>;\n\""));
				}

				found = upperLine.find("UDP");
				if (found != std::string::npos){
					tmpMsg = line.substr(found + 3);
					position = tmpMsg.find_first_of(" ");
					tmpMsg = tmpMsg.substr(position + 1);
					vals.returnIP = tmpMsg.substr(0, tmpMsg.find_first_of(":;"));

					found = tmpMsg.find(":");
					if (found != std::string::npos){
						tmpMsg = tmpMsg.substr(found + 1);
						vals.returnPort = tmpMsg.substr(0, tmpMsg.find_first_of(";"));
					}
				}

			}
			else if (!strncmp(upperLine.c_str(), "TO", 2)){
				position = line.find_first_of("<>;\n\"");
				tmpMsg = line.substr(position + 1);
				vals.to = tmpMsg.substr(0, tmpMsg.find_first_of("<>;\n\""));

				std::size_t found = upperLine.find("TAG=");
				if (found != std::string::npos){
					tmpMsg = line.substr(found + 4);
					vals.toTag = tmpMsg.substr(0, tmpMsg.find_first_of("\r\n;"));
				}
			}
			else if (!strncmp(upperLine.c_str(), "CSEQ", 4)){
				position = line.find_first_of(" ");
				tmpMsg = line.substr(position + 1);
				vals.cSeq = tmpMsg.substr(0, tmpMsg.find_first_of(" "));
			}
			else if (!strncmp(upperLine.c_str(), "CONTENT-LENGTH", 14)) {
				position = line.find_first_of(" ");
				tmpMsg = line.substr(position + 1);
				vals.contentLength = tmpMsg.substr(0, tmpMsg.find_first_of("\r\n;"));
			}
			else if (!strncmp(upperLine.c_str(), "CALL-ID:", 8)){
				position = line.find_first_of(" ");
				tmpMsg = line.substr(position + 1);
				vals.callID = tmpMsg.substr(0, tmpMsg.find_first_of("\r\n;"));

			}
			else if (!strncmp(upperLine.c_str(), "EXPIRES:", 8)) {
				position = line.find_first_of(" ");
				tmpMsg = line.substr(position + 1);
				vals.expires = tmpMsg.substr(0, tmpMsg.find_first_of("\r\n;"));
			}
			else if (!strncmp(upperLine.c_str(), "WWW-AUTHENTICATE", 16)){

				std::size_t found = upperLine.find("REALM=\"");
				if (found != std::string::npos){
					tmpMsg = line.substr(found + 7);
					vals.realm = tmpMsg.substr(0, tmpMsg.find_first_of("<>;\n\""));
				}

				found = upperLine.find("NONCE=\"");
				if (found != std::string::npos){
					tmpMsg = line.substr(found + 7);
					vals.nonce = tmpMsg.substr(0, tmpMsg.find_first_of("<>;\n\""));
				}

			}
		}
	}

	string seqNumber = intToString(this->sNumber);

	for (unsigned int i = 0; i < this->msgstats.size(); ++i) {
		if (this->msgstats[i].branch == vals.branch && codeMsg != "MESSAGE") {
			seqNumber = this->msgstats[i].identifier;
			this->msgstats.erase(this->msgstats.begin() + i);
			break;
		}
	}

	// if I am not the reciever of message, ignore
	if (action == A_MESSAGE) {
		position = wholeMessage.find("\r\n\r\n");
		if (isNumber(vals.contentLength)) {
			message = wholeMessage.substr(position + 4, atoi(vals.contentLength.c_str()));
		}
	}

	// remove "sip:" prefix
	string upper = myToUpper(vals.from);
	string from = vals.from;
	string to = vals.to;
	if (!strncmp(upper.c_str(), "SIP:", 4)) {
		from = removeAndPrint(vals.from.substr(4));
	}

	upper = myToUpper(vals.to);
	if (!strncmp(upper.c_str(), "SIP:", 4)) {
		to = removeAndPrint(vals.to.substr(4));
	}

	cout << seqNumber << " " << ((code.length() != 0) ? code + " " : "") << ((codeMsg.length() != 0) ? codeMsg : "") << " " << (isResponse? to : from) << " " << (isResponse? from : to) << (message.length() ? " " + message : "") << endl;
	cout.flush();	

	switch (action) {
		case A_REGISTER:
			responseToServer = this->getClientRegisterAuthorize(vals);
			break;

		case A_MESSAGE:
			// response only when I am the reciever
			if (to.find(this->profile.userName + "@") == std::string::npos) {
				responseToServer = this->getMMessageErrReply(vals);
			}
			else {
				responseToServer = this->getMMessageReply(vals);
			}
			break;

		case A_ERR_500:
			if (!this->isRegistered) {
				this->TrynextLoginAttempt = true;
			}
			break;
		case A_REGISTER_CONFIRMED:
			if (vals.expires.length()) {
				// check if valid number and convert to int.. otherwise get profile value
				if (isNumber(vals.expires)) {
					this->nextRegisterIn = atoi(vals.expires.c_str());
				}
				else {
					this->nextRegisterIn = this->profile.expire;
				}
			}
			break;
		default:
			break;
	}

	return responseToServer;
}

/**
* initialize the client
* @return error code
*/
Errors Client::init() {
	struct hostent *hostent;

	// get local ip address
	if (this->profile.server == "127.0.0.1" || this->profile.server == "localhost" || this->profile.server == "lo"){
		// if target ip is loopback set my ip to loopback as well
		this->clientIP = "127.0.0.1";
	}
	else{
		struct ifaddrs *ifaddr, *ifa;
		char host[NI_MAXHOST];

		// get list of interfaces
		if (getifaddrs(&ifaddr) == -1) { 
			return IPERR;
		}

		// loop through each interface
		for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {

			// ignore interface without address
			if (ifa->ifa_addr == NULL){
				continue;
			}
			// ignore loopback interface
			if (!strcmp(ifa->ifa_name, "lo")){
				continue;
			}

			// looking for IPv4 address
			if (ifa->ifa_addr->sa_family == AF_INET){
				if (getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in), host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST) != 0) {
					continue;
				}
				// ignore loopback address
				if (!strcmp(host, "127.0.0.1") || !strcmp(host, "localhost")){
					continue;
				}
				this->clientIP = string(host);
			}
		}

		freeifaddrs(ifaddr);

		// if no ip found return error
		if (this->clientIP.length() == 0) {
			return IPERR;
		}
	}

	memset(&this->server, 0, sizeof(this->server));

	int srvrPort = 5060;
	if (this->profile.port != "") {
		if (isNumber(this->profile.port)) {
			srvrPort = atoi(this->profile.port.c_str());
		}
		else {
			return INITERR;
		}
	}

	this->server.sin_family = AF_INET;
	this->server.sin_port = htons(srvrPort);

	if ((hostent = gethostbyname(this->profile.server.c_str())) == NULL) {
		return INITERR;
	}

	memcpy(&this->server.sin_addr, hostent->h_addr, hostent->h_length);

	this->client.sin_family = AF_INET;
	this->client.sin_port = htons(PORT);

	if ((this->clientSocket = socket(AF_INET, SOCK_DGRAM, 0)) == -1 ) {
		return SERR;
	}
	
	// set socket nonblocking
	if (fcntl(this->clientSocket, F_SETFL, fcntl(this->clientSocket, F_GETFL) | O_NONBLOCK) != 0) {
		return SERR;
	}

	memset(&this->client, 0, sizeof(this->client));

	this->client.sin_family = AF_INET;
	this->client.sin_port = htons(PORT);
	this->client.sin_addr.s_addr = htonl(INADDR_ANY);

	int port = PORT;
	int result = bind(this->clientSocket, (struct sockaddr *) &this->client, sizeof(this->client));

	// do until we find empty port or the port is lesser than 5090
	while (result == -1 && port < 5090) {
		++port;
		this->client.sin_port = htons(port);
		result = bind(this->clientSocket, (struct sockaddr *) &this->client, sizeof(this->client));
	}

	if (result == -1){
		return SERR;
	}
	
	// get port
	this->clientPort = intToString(port);

	// initialize suffix
	this->suffix = "-" + randomString(4) + "-" + randomString(4) + "-" + randomString(4) + "-" + randomString(12);

	return OK;
}

string intToString(int number){
	stringstream tmp;
	tmp << number;
	string myString;
	tmp >> myString;

	return myString;
}

string myToUpper(string lowerCase) {
	string upperCase = "";
	char c;

	for (string::size_type index = 0; index < lowerCase.length(); index++) {
		c = toupper(lowerCase[index]);
		upperCase += c;
	}

	return upperCase;
}

// hash string
string hashMD5(string str) {
	string result;
	const char* test = str.c_str();

	MD5_CTX md5;

	MD5_Init(&md5);
	MD5_Update(&md5, (const unsigned char *)test, str.length());
	unsigned char buffer_md5[16];
	MD5_Final(buffer_md5, &md5);

	result.reserve(32); 

	for (std::size_t i = 0; i != 16; ++i) {
		result += "0123456789abcdef"[buffer_md5[i] / 16];
		result += "0123456789abcdef"[buffer_md5[i] % 16];
	}

	return result;
}

// generate random string
// source: http://stackoverflow.com/questions/440133/how-do-i-create-a-random-alpha-numeric-string-in-c
string randomString(const int len) {
	string str = "";
	static const char alphanum[] =
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";

	for (int i = 0; i < len; ++i) {
		str += alphanum[rand() % (sizeof(alphanum)-1)];
	}
	return str;
}

// print info about sent message
void Client::printSentMessage(string number, string request, string from, string to, string message) {

	cout << number + " " + request + " " + from + " " + to + (message.length() ? (" " + message) : "") << endl;
	cout.flush();
}

// check if string is valid number
bool isNumber(string val) {
	std::string::const_iterator it = val.begin();
	while (it != val.end() && std::isdigit(*it)) {
		++it;
	}
	return !val.empty() && it == val.end();
}
// print port if not 5060
string printPort(string port) {
	return (port == "5060" || port == "") ? "" : ":" + port;
}

// remove port 5060 from address
string removeAndPrint(string address) {
	std::size_t found;
	string port;
	string adr = address;
	if ((found = address.find(":")) != std::string::npos) {
		port = address.substr(found + 1);
		adr = address.substr(0, found);
		adr += (port == "5060") ? "" : ":" + port;
	}
	return adr;
}


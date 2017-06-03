/**
* ISA project
* Name: Klient SIP
* Author: Lukas Pelanek, xpelan03@stud.fit.vutbr.cz, 2015
**/

#include "Parameters.h"
#include "Message.h"

/**
* Get parameters
* @return struct tParams filled with data
*/
Errors getParams(tProfile *profile, vector<Message*> *messages, int argc, char **argv) {
	
	if (argc == 2) {
		if ((strcmp(argv[1], "-h") == 0) || (strcmp(argv[1], "--help") == 0)) {
			cout << "napoveda" << endl;
			exit(OK);
		}
		else {
			return PARAMSERR;
		}
	}
	else if (argc < 3){
		return PARAMSERR;
	}
	else if (argc == 3){
		if ((strcmp(argv[1], "-p") == 0)) {
			return getProfile(profile, argv[2]);
		}
		else {
			return PARAMSERR;
		}
	}
	else if (argc == 5){
		if ((strcmp(argv[1], "-p") == 0) && (strcmp(argv[3], "-m") == 0)) {
			if (getProfile(profile, argv[2]) == OK){
				return getMessages(messages, argv[4]);
			}
			else{
				return FILEERR;
			}
		}
		else if ((strcmp(argv[3], "-p") == 0) && (strcmp(argv[1], "-m") == 0)) {
			if (getProfile(profile, argv[4]) == OK){
				return getMessages(messages, argv[2]);
			}
			else{
				return FILEERR;
			}
		}
		else {
			return PARAMSERR;
		}
	}
	else{
		return PARAMSERR;
	}

	return OK;
}

/**
* Get profile
* @return error code
*/
Errors getProfile(tProfile *profile, char *path) {
	ifstream file(path);
	string line;
	string tmp;
	if ( ! file){
		return FOPENERR;
	}
	else{
		string::size_type position;

		while (getline(file, line)) {
			if (!strncmp(line.c_str(), "server=", 7)) {
				tmp = line.substr(7);
				if ((position = tmp.find_first_of("\r\n")) != std::string::npos) {
					tmp = tmp.substr(0, position);
				}
				if ((position = tmp.find(":")) != std::string::npos) {
					profile->server = tmp.substr(0, position);
					profile->port = tmp.substr(position + 1);
				}
				else {
					profile->server = tmp;
				}
			}
			else if (!strncmp(line.c_str(), "username=", 9)) {
				tmp = line.substr(9);
				if ((position = tmp.find_first_of("\r\n")) != std::string::npos) {
					tmp = tmp.substr(0, position);
				}
				profile->userName = tmp;
			}
			else if (!strncmp(line.c_str(), "password=", 9)) {
				tmp = line.substr(9);
				if ((position = tmp.find_first_of("\r\n")) != std::string::npos) {
					tmp = tmp.substr(0, position);
				}
				profile->password = tmp;
			}
			else if (!strncmp(line.c_str(), "expires=", 8)) {
				tmp = line.substr(8);
				if ((position = tmp.find_first_of("\r\n")) != std::string::npos) {
					tmp = tmp.substr(0, position);
				}
				if (isInteger(tmp)) {
					profile->expire = atoi(tmp.c_str());
				}
			}

		}

		file.close();

		// check if necessary parameters are set
		if (profile->server == "" || profile->userName == "" || profile->password == "") {
			return FILEERR;
		}

		return OK;
	}
}

/**
* Get messages
* @return error code
*/
Errors getMessages(vector<Message*> *messages, char *path) {
	ifstream file(path);
	string line;
	if (!file){
		return FOPENERR;
	}
	else{
		while (getline(file, line)) {

			// skip empty lines
			if (line.length() == 0) {
				continue;
			}

			string address = "";
			string port = "";
			string message = "";
			string rawIP = "";

			// get end of string
			string::iterator end = line.end();
			std::size_t found;

			if ((found = line.find_first_of(" ")) == std::string::npos) {
				file.close();
				return FILEERR;
			}
			else {
				// store message
				message = line.substr(found + 1);

				// get data until message
				address = line.substr(0, found);

				if ((found = address.find("@")) != std::string::npos) {
					rawIP = address.substr(found + 1);
				}

				if ((found = rawIP.find(":")) != std::string::npos) {
					port = rawIP.substr(found + 1);
					rawIP = rawIP.substr(0, found);
				}

			}
			if (address.length() && message.length()) {
				if (port != "" && !isInteger(port.c_str())) {
					file.close();
					return FILEERR;
				}
				messages->push_back(new Message(address, port, message, rawIP));
			}
			else{
				file.close();
				return FILEERR;
			}
		}

		file.close();
		return OK;
	}
}

inline bool isInteger(const std::string & s) {
	if (s.empty() || ((!isdigit(s[0])))) {
		return false;
	}
	char * p;
	strtol(s.c_str(), &p, 10);

	return (*p == 0);
}



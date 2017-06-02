/*
* Soubor:  server.cpp
* Datum:   2015/02/04
* Autor:   Lukas Pelanek, xpelan03@stud.fit.vutbr.cz
* Projekt: Program pro vyhledani informaci o uzivatelich Unixoveho OS
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

// struktura ve ktere uchovavam zaznamy
struct myData{
	string login;
	string userID;
	string groupID;
	string name;
	string home;
	string shell;
};

// zpracovani parametru prikazove radky
int getPort(int argc, char **argv){
	int c, port;
	while ((c = getopt(argc, argv, "p:")) != -1) {
		switch (c) {
			case 'p':
				port = atoi(optarg);
				break;
			default:
				break;
		}
	}
	return port;
}
// ukonceni neaktivnich procesu -handler-
void killZombie(int signal){
	wait(&signal);
}

//rozdeleni radku na jednotlive stringy
vector<string> split(string str){
	vector<string> tmp;
	string tmpStr;
	std::string::size_type size = str.size();
	for (std::string::size_type i = 0; i < size; ++i) {
		if (str[i] == ':'){
			tmp.push_back(tmpStr);
			tmpStr = "";
			continue;
		}
		if (i == (size -1)){
			tmpStr += str[i];
			tmp.push_back(tmpStr);
			return tmp;
		}
		tmpStr += str[i];
	}
	return tmp;
}

bool strEqual(const string& x, const string& y)
{
    unsigned int size = x.size();
    if (y.size() != size)
        return false;
    for (unsigned int i = 0; i < size; ++i)
        if (tolower(x[i]) != tolower(y[i]))
            return false;
    return true;
}


int main(int argc, char* argv[]){
	if (argc < 2){
		fprintf(stderr, "Chyba: nespravne zadane argumenty programu.\n"); 
		return EXIT_FAILURE; 
	}

	signal(SIGCHLD, killZombie);

	int port = getPort(argc, argv);						// port na kterem bezi server, musi byt v rozsahu 1 - 65535
	int s;												// socket
	int newSocket;										// file descriptor pripojeneho klienta
	struct sockaddr_in svr;
	socklen_t svrLen;


	if (port < 1 || port > 65535){
		fprintf(stderr, "Chyba: zadany port musi byt v rozsahu 1 - 65535.\n");
		return EXIT_FAILURE;
	}
	//vytvorim socket
	if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0){
		fprintf(stderr, "Chyba: nepodarilo se otevrit socket.\n");
		return EXIT_FAILURE;
	}

	svr.sin_family = PF_INET;              
	svr.sin_port = htons(port);
	svr.sin_addr.s_addr = INADDR_ANY;  

	//prirazeni adresy
	if ((bind(s, (struct sockaddr *)&svr, sizeof(svr))) < 0){
		fprintf(stderr, "Chyba: chyba pri volani funkce bind().\n");
		return EXIT_FAILURE;
	}

	//cekani na prichozi socket, fronta
	if (listen(s, 5)){
		fprintf(stderr, "Chyba: chyba pri otevirani socketu.\n");
		return EXIT_FAILURE;
	}

	svrLen = sizeof(svr);

	

	while (true){
		if ((newSocket = accept(s, (struct sockaddr *) &svr, &svrLen)) < 0) {
			fprintf(stderr, "Chyba: chyba pri vytvareni socketu.\n");
			return EXIT_FAILURE;
		}

		int newProcess = fork();

		if (newProcess == 0){
			close(s);				//uzavru socket

			char buf[4096];
			//bzero(buf, sizeof(buf));
			int length;
			while (true){
				length = read(newSocket, buf, sizeof(buf));

				if (length < 0){
					fprintf(stderr, "Chyba: chyba pri cteni.\n");
					return EXIT_FAILURE;
				}
				else if (length > 0){
					break;
				}
			}

			string params;
			string outMsg;
			bool loop = true;
			std::vector<string> IDs;
			string tmp;
			std::vector<myData> users;
			bool L = false, U = false, G = false, N = false, H = false, S = false, l = false, u = false;
			int index = 0;
			int state = 0;

			// otevreni databaze a kontrola zda bylo uspesne
			ifstream etcPasswd("/etc/passwd");
			if (!etcPasswd.is_open()){
				outMsg.append("E$a");
				if ((write(newSocket, outMsg.c_str(), outMsg.length())) < 0){
					fprintf(stderr, "Chyba: chyba pri odesilani dat.\n");
					return EXIT_FAILURE;
				}
				fprintf(stderr, "Chyba: chybny format databze.\n");
				return EXIT_FAILURE;
			}

			//automat zpracujici prichozi zpravu
			while (loop){
				string tmpStr;
				char c = buf[index];

				switch (state){
				case 0:
					if (c == 'l'){
						l = true;
						state = 1;
					}
					else if (c == 'u'){
						u = true;
						state = 1;
					}
					else{
						outMsg.append("E$a");
						if ((write(newSocket, outMsg.c_str(), outMsg.length())) < 0){
							fprintf(stderr, "Chyba: chyba pri odesilani dat.\n");
							return EXIT_FAILURE;
						}
						return EXIT_SUCCESS;
					}
					break;
				case 1:
					if (c == 'L'){
						L = true;
						params += 'L';
						break;
					}
					else if (c == 'U'){
						U = true;
						params += 'U';
						break;
					}
					else if (c == 'G'){
						G = true;
						params += 'G';
						break;
					}
					else if (c == 'N'){
						N = true;
						params += 'N';
						break;
					}
					else if (c == 'H'){
						H = true;
						params += 'H';
						break;
					}
					else if (c == 'S'){
						S = true;
						params += 'S';
						break;
					}
					else if (c == '\n'){
						state = 2;
						break;
					}
					else{
						outMsg.append("E$a");
						if ((write(newSocket, outMsg.c_str(), outMsg.length())) < 0){
							fprintf(stderr, "Chyba: chyba pri odesilani dat.\n");
							return EXIT_FAILURE;
						}
						return EXIT_SUCCESS;
					}
				case 2:

					if (c == '$'){
						state = 3;
						break;
					}

					
					while (c != '\n'){
						c = buf[index];
						if (c != '\n'){
							tmpStr += c;
						}
						index++;
					}
					index--;
					IDs.push_back(tmpStr);
					tmpStr = "";
					break;

				case 3:
					if (buf[index] == 'e'){
						index++;
						if (buf[index] == '$'){
							loop = false;
							break;
						}
					}
					outMsg.append("E$a");
					if ((write(newSocket, outMsg.c_str(), outMsg.length())) < 0){
						fprintf(stderr, "Chyba: chyba pri odesilani dat.\n");
						return EXIT_FAILURE;
					}
					return EXIT_SUCCESS;
				}
				index++;
			}
			myData tmpData;
			while (getline(etcPasswd, tmp)){
				vector<string> rozdeleno = split(tmp);

				if (rozdeleno.size() != 7 && rozdeleno.size() != 6){
					outMsg.append("E$a");
					if ((write(newSocket, outMsg.c_str(), outMsg.length())) < 0){
						fprintf(stderr, "Chyba: chyba pri odesilani dat.\n");
						return EXIT_FAILURE;
					}
					fprintf(stderr, "Chyba: chybny format databze.\n");
					return EXIT_FAILURE;
				}
				
				for (unsigned int x = 0; x < rozdeleno.size(); x++){

					if (x == 0){
						tmpData.login = rozdeleno[x];
					}
					else if (x == 2){
						tmpData.userID = rozdeleno[x];
					}
					else if (x == 3){
						tmpData.groupID = rozdeleno[x];
					}
					else if (x == 4){
						tmpData.name = rozdeleno[x];
					}
					else if (x == 5){
						tmpData.home = rozdeleno[x];
					}
					else if (x == 6){
						tmpData.shell = rozdeleno[x];
					}
				}
				for (unsigned int x = 0; x < IDs.size(); x++){
					if (l){
						if (strEqual(IDs[x], tmpData.login)){
							users.push_back(tmpData);
						}
					}
					else if (u){
						if (strEqual(IDs[x], tmpData.userID)){
							users.push_back(tmpData);
						}
					}
				}	
			}
			// vytvoreni zpravy pro klienta
			for (unsigned int x = 0; x < IDs.size(); x++){
				bool found = false;
				for (unsigned int y = 0; y < users.size(); y++){
					if (l){
						if (strEqual(users[y].login, IDs[x])){
							found = true;
							outMsg += "O$";
							for (std::string::size_type i = 0; i < params.size(); ++i){
								if (params[i] == 'L'){
									outMsg.append(users[y].login);
									if (i == params.size() - 1){
										outMsg += '\n';
									}
									else{
										outMsg += ' ';
									}
								}
								else if (params[i] == 'U'){
									outMsg.append(users[y].userID);
									if (i == params.size() - 1){
										outMsg += '\n';
									}
									else{
										outMsg += ' ';
									}
								}
								else if (params[i] == 'G'){
									outMsg.append(users[y].groupID);
									if (i == params.size() - 1){
										outMsg += '\n';
									}
									else{
										outMsg += ' ';
									}
								}
								else if (params[i] == 'N'){
									outMsg.append(users[y].name);
									if (i == params.size() - 1){
										outMsg += '\n';
									}
									else{
										outMsg += ' ';
									}
								}
								else if (params[i] == 'H'){
									outMsg.append(users[y].home);
									if (i == params.size() - 1){
										outMsg += '\n';
									}
									else{
										outMsg += ' ';
									}
								}
								else if (params[i] == 'S'){
									outMsg.append(users[y].shell);
									if (i == params.size() - 1){
										outMsg += '\n';
									}
									else{
										outMsg += ' ';
									}
								}
							}
							break;
						}
					}
					else if (u){
						if (!strcmp(IDs[x].c_str(), users[y].userID.c_str())){
							found = true;
							outMsg += "O$";
							for (std::string::size_type i = 0; i < params.size(); ++i){
								if (params[i] == 'L'){
									outMsg.append(users[y].login);
									if (i == params.size() - 1){
										outMsg += '\n';
									}
									else{
										outMsg += ' ';
									}
								}
								else if (params[i] == 'U'){
									outMsg.append(users[y].userID);
									if (i == params.size() - 1){
										outMsg += '\n';
									}
									else{
										outMsg += ' ';
									}
								}
								else if (params[i] == 'G'){
									outMsg.append(users[y].groupID);
									if (i == params.size() - 1){
										outMsg += '\n';
									}
									else{
										outMsg += ' ';
									}
								}
								else if (params[i] == 'N'){
									outMsg.append(users[y].name);
									if (i == params.size() - 1){
										outMsg += '\n';
									}
									else{
										outMsg += ' ';
									}
								}
								else if (params[i] == 'H'){
									outMsg.append(users[y].home);
									if (i == params.size() - 1){
										outMsg += '\n';
									}
									else{
										outMsg += ' ';
									}
								}
								else if (params[i] == 'S'){
									outMsg.append(users[y].shell);
									if (i == params.size() - 1){
										outMsg += '\n';
									}
									else{
										outMsg += ' ';
									}
								}
							}
							break;
						}
					}
					
				}
				if (!found){
					outMsg.append("E$b").append(IDs[x]).append("\n");
				}
			}
			// odeslu zpracovana data klientovi
			if ((write(newSocket, outMsg.c_str(), outMsg.length())) < 0){
				fprintf(stderr, "Chyba: chyba pri odesilani dat.\n");
				return EXIT_FAILURE;
			}
			close(newSocket);
			

			return EXIT_SUCCESS;

		}
		// pokud se nezdaril fork, uzavru sockety a ukoncim s chybou
		else if (newProcess < 0){
			close(s);
			close(newSocket);
			fprintf(stderr, "Chyba: chyba pri vytvareni noveho procesu.\n");
			return EXIT_FAILURE;
		}
		close(newSocket);

	}


	return 0;
}
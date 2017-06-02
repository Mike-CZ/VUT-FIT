/*
* Soubor:  client.cpp
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
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>

#include <string>
#include <iostream>

using namespace std;

int getParams(int argc, char **argv, int *port, string *host, string *key, string *params, string *outMsg, bool *l, bool *u){
	int ch;
	bool p = false, h = false, L = false, U = false, G = false, N = false, H = false, S = false;

	//zpracovani parametru
	while ((ch = getopt(argc, argv, "p:l:h:u:LUGNHS")) != -1){
		switch (ch){
		case 'p':
			if (!p){
				*port = atoi(optarg);
				p = true;
			}
			else{
				return EXIT_FAILURE;
			}
			break;
		case 'h':
			if (!h){
				(*host).append(optarg);
				h = true;
				
			}
			else{
				return EXIT_FAILURE;
			}
			break;
		case 'l':
			optind--;
			*outMsg = "";
			*key = "";
			(*outMsg).append("l");
				for (; optind < argc && *argv[optind] != '-'; optind++){
					(*key).append(argv[optind]);
					(*key).append("\n");
				}
			*l = true;
			
			break;
		case 'u':
			optind--;
			*outMsg = "";
			*key = "";
			(*outMsg).append("u");
				for (; optind < argc && *argv[optind] != '-'; optind++){
					(*key).append(argv[optind]);
					(*key).append("\n");
				}
			*u = true;

			break;
		case 'L':
			if (!L){
				(*params).append("L");
				L = true;
			}
			else{
				return EXIT_FAILURE;
			}
			break;
		case 'U':
			if (!U){
				(*params).append("U");
				U = true;
			}
			else{
				return EXIT_FAILURE;
			}
			break;
		case 'G':
			if (!G){
				(*params).append("G");
				G = true;
			}
			else{
				return EXIT_FAILURE;
			}
			break;
		case 'N':
			if (!N){
				(*params).append("N");
				N = true;
			}
			else{
				return EXIT_FAILURE;
			}
			break;
		case 'H':
			if (!H){
				(*params).append("H");
				H = true;
			}
			else{
				return EXIT_FAILURE;
			}
			break;
		case 'S':
			if (!S){
				(*params).append("S");
				S = true;
			}
			else{
				return EXIT_FAILURE;
			}
			break;

		default:
			break;

		}
	}
	// pokud neni zadan povinny parametr nebo chyba kombinace -> chyba
	if ((!p) || (!h) || ((!(*l)) && (!(*u))) || ((!L) && (!U) && (!G) && (!N) && (!H) && (!S))){
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}


int main(int argc, char* argv[]){
	if (argc < 3){
		fprintf(stderr, "Chyba: nespravne zadane argumenty programu.\n");
		return EXIT_FAILURE;
	}
	int s;
	bool l = false, u = false;
	struct sockaddr_in addr; 
	struct hostent *info;
	int port;
	string host;
	string key;
	string params;
	string outMsg;
	char buffer[4096];

	if (getParams(argc, argv, &port, &host, &key, &params, &outMsg, &l, &u) == EXIT_FAILURE){
		fprintf(stderr, "Chyba: chybne parametry.\n");
		return EXIT_FAILURE;
	}
	// sloucim zpracovane parametry a vytvorim zpravu pro server
	outMsg.append(params).append("\n").append(key).append("$e$");

	//vytvorim socket
	if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0){
		fprintf(stderr, "Chyba: nepodarilo se otevrit socket.\n");
		return EXIT_FAILURE;
	}


	addr.sin_family = PF_INET;              
	addr.sin_port = htons(port);  

	// prelozeni adresy
	if ((info = gethostbyname(host.c_str())) == NULL){
		fprintf(stderr, "Chyba: neplatna adresa.\n");
		return EXIT_FAILURE;
	}
	memcpy(&addr.sin_addr, info->h_addr, info->h_length);

	// pripojeni k serveru
	if ((connect(s, (struct sockaddr *)&addr, sizeof(addr))) < 0){
		fprintf(stderr, "Chyba: nepodarilo se pripojit k serveru.\n");
		return EXIT_FAILURE;
	}

	//odeslani pozadavku serveru
	if((write(s, outMsg.c_str(), outMsg.length())) < 0){
		fprintf(stderr, "Chyba: chyba pri odesilani.\n");
		return EXIT_FAILURE;
	}

	int length;
	while (true){
		length = read(s, buffer, sizeof(buffer));
		if (length < 0){
			fprintf(stderr, "Chyba: chyba pri cteni zpravy.\n");
			return EXIT_FAILURE;
		}
		else if (length > 0){
			break;
		}

	}
	string inMSG = buffer;
	string rcvMsg;
	unsigned int index = 0;
	int state = 0;

	//konecny automat dekodujici zpravu
	while (index < inMSG.size()){

		switch (state){
		case 0:
			if (inMSG[index] == 'O'){
				index++;
				if (inMSG[index] == '$'){
					state = 1;
					break;
				}
			}
			else if (inMSG[index] == 'E'){
				index++;
				if (inMSG[index] == '$'){
					state = 2;
					break;
				}
			}
			break;

		case 1:
			while (inMSG[index] != '\n'){
				rcvMsg += inMSG[index];
				index++;
			}
			if (index +1 < inMSG.size()){
				cout << rcvMsg << endl;
			}
			else{
				cout << rcvMsg;
			}
			
			rcvMsg = "";
			state = 0;
			break;

		case 2:
			if (inMSG[index] == 'a'){
				fprintf(stderr, "Chyba: neznama odpoved.\n");
				return EXIT_FAILURE;
			}
			else if (inMSG[index] == 'b'){
				index++;
				while (inMSG[index] != '\n'){
					rcvMsg += inMSG[index];
					index++;
				}
				if (l){
					fprintf(stderr, "Chyba: neznamy login %s.\n", rcvMsg.c_str());
				}
				else if (u){
					fprintf(stderr, "Chyba: nezname uzivatelske ID %s.\n", rcvMsg.c_str());
				}
				state = 0;
				rcvMsg = "";
				break;
			}
		}


		index++;
	}

	if (close(s) < 0) {
		fprintf(stderr, "Chyba: nepovedlo se uzavrit socket.\n");
		return EXIT_FAILURE;
	}




	return 0;
}
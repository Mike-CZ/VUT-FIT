/*
* Soubor:  Intercepter.h
* Datum:   2017/4/20
* Autor:   Bc. Lukas Pelanek, xpelan03@stud.fit.vutbr.cz
* Projekt: PDS, Man in the Middle
* Popis:   Aplikace pro preposilani provozu.
*/


#ifndef INTERCEPTER_H
#define INTERCEPTER_H

#include <csignal>
#include <fstream>
#include <algorithm>
#include <cstdint>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <ifaddrs.h>
#include <netinet/in.h> 
#include <locale>
#include <net/if.h>
#include <unistd.h>
#include <string.h> 
#include <arpa/inet.h>
#include <vector>
#include <cstring>
#include <sys/socket.h>
#include <netinet/ip6.h>      
#include <netinet/icmp6.h>   
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <netinet/ip.h>

#include "Packets.h"
#include "IOHandler.h"

/* Pri implementaci jsem vychazel ze souboru, ktere jsou ke stazeni na teto adrese: http://www.pdbuchan.com/rawsock/rawsock.html */

void signalHandler(int signum);

typedef struct {
	std::string mac;
	std::string group;
	std::vector<std::string> ipv4Addresses;
	std::vector<std::string> ipv6Addresses;
} I_node;

typedef struct {
	uint8_t mac1[6];
	uint8_t mac2[6];
	std::vector<struct in_addr> ipv4Addresses1;
	std::vector<struct in6_addr> ipv6Addresses1;
	std::vector<struct in_addr> ipv4Addresses2;
	std::vector<struct in6_addr> ipv6Addresses2;
} t_node;

class Intercepter {
private:
	const t_scannerParams _params;
	std::vector<t_node> _pairs;
	int *_socket;
	struct sockaddr_ll _dev;
	struct in_addr _ipv4Addr;
	struct in_addr _ipv4Mask;
	struct in6_addr _ipv6Addr;
	struct in6_addr _ipv6Mask;
	uint8_t _macAddress[6];

	int _init();
	int _readInputXML();
	std::string _removeSpaces(std::string in);
	std::string _toLower(std::string word);
	std::string _readToken(std::string *inString);
	int _addPair(I_node p1, I_node p2);
	int _intercept(uint8_t *packet);
	bool _checkIpv4Addr(std::vector<struct in_addr> &ipv4Addresses, uint32_t &address);
	bool _checkIpv6Addr(std::vector<struct in6_addr> &ipv6Addresses, struct in6_addr &address);

public:
	Intercepter(t_scannerParams params, int *socket);
	int start();
};


#endif

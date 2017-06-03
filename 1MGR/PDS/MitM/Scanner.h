/*
* Soubor:  Scanner.h
* Datum:   2017/4/20
* Autor:   Bc. Lukas Pelanek, xpelan03@stud.fit.vutbr.cz
* Projekt: PDS, Man in the Middle
* Popis:   Aplikace skenujici lokalni sit pomoci ARP a NDP protokolu
*/

#ifndef SCANNER_H
#define SCANNER_H

#include <csignal>
#include <fstream>
#include <cstdint>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <ifaddrs.h>
#include <netinet/in.h> 
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

typedef struct _node {
	uint8_t mac[6];
	std::vector<in_addr> ipv4Addresses;
	std::vector<in6_addr> ipv6Addresses;
}t_Node;

class Scanner {
private:
	const std::string _interface;
	int *_socket;
	struct in_addr _ipv4Addr;
	struct sockaddr_ll _dev;
	struct in_addr _ipv4Mask;
	struct in6_addr _ipv6Addr;
	struct in6_addr _ipv6Mask;
	std::vector<t_Node> *_nodes;
	uint8_t _macAddress[6];

	int _init();
	int _sendArpRequests();
	int _receiveArpResponds();
	int _sendICMPv6Packet();
	int _receiveICMPv6Packets();
	uint16_t _getChecksum(uint8_t *packet, int dataLength);
	void _insertIPv4Node(uint8_t *mac, in_addr *ip);
	void _insertIPv6Node(uint8_t *mac, in6_addr ip);

public:
	Scanner(std::string inf, int *sock, std::vector<t_Node> *nodes);
	static int printNodesIntoXml(std::vector<t_Node> *nodes, std::ofstream *outputFile);
	int start();
};

#endif
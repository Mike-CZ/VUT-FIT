/*
* Soubor:  IOHandler.cpp
* Datum:   2017/4/20
* Autor:   Bc. Lukas Pelanek, xpelan03@stud.fit.vutbr.cz
* Projekt: PDS, Man in the Middle
* Popis:   Sada funkci pro praci se vstupnimi a vystupnimi daty
*/


#include "IOHandler.h"

namespace IOHandler {

	/* Otevre soubor pro zapis */
	int openOutputFile(std::string fileName, std::ofstream *stream) {
		stream->open(fileName.c_str(), std::ofstream::out);
		return (stream->is_open() == true) ? 0 : -1;
	}

	/* Zavre soubor pro zapis */
	void closeOutputFile(std::ofstream *stream) {
		stream->close();
	}

	/* Funkce nacte vstupni parametry pro spofer */
	int getSpooferParameters(int argc, char **argv, t_spooferParams *params) {
		int errCode = ERR_NO_ERROR;
		int opt;

		static struct option long_options[] =
		{
			{ "victim1ip", required_argument, NULL,  '1' },
			{ "victim1mac", required_argument, NULL, '2' },
			{ "victim2ip", required_argument, NULL, '3' },
			{ "victim2mac", required_argument, NULL, '4' },
		};

		// kontrola poctu vsupnich parametru
		if (argc != 15) {
			return ERR_ARGUMENT_COUNT;
		}

		// inicializujeme casovac
		params->timer = -1;

		while ((opt = getopt_long_only(argc, argv, "i:t:p:", long_options, NULL)) != -1) {
			switch (opt) {
			case 'i':
				params->interface = optarg;
				break;
			case 't':
				params->timer = std::stoi(optarg);
				break;
			case 'p':
				params->protocol = optarg;
				break;
			case '1':
				params->victimIP = optarg;
				break;
			case '2':
				params->victimMAC = optarg;
				break;
			case '3':
				params->victimIP2 = optarg;
				break;
			case '4':
				params->victimMAC2 = optarg;
				break;
			case '?':
				return ERR_UNKNOWN_ARGUMENT;
			}
		}

		// kontrola nactenych hodnot
		if (params->interface.length() <= 0 || params->victimIP.length() <= 0 || params->victimMAC.length() <= 0 || params->victimIP2.length() <= 0 ||
			params->victimMAC2.length() <= 0 || params->protocol.length() <= 0 || params->timer == -1) {
			return ERR_UNKNOWN_ARGUMENT;
		}

		if (params->protocol != "arp" && params->protocol != "ndp") {
			return ERR_UNKNOWN_ARGUMENT;
		}

		if (params->victimMAC.size() != 14 || params->victimMAC2.size() != 14) {
			return ERR_UNKNOWN_ARGUMENT;
		}

		if (params->timer < 0) {
			return ERR_UNKNOWN_ARGUMENT;
		}

		return ERR_NO_ERROR;
	}

	/* Funkce nacte vstupni parametry pro scanner */
	int getChooserParameters(int argc, char **argv, t_scannerParams *params) {
		int errCode = ERR_NO_ERROR;
		int opt;

		// kontrola poctu vsupnich parametru
		if (argc != 5) {
			return ERR_ARGUMENT_COUNT;
		}

		while ((opt = getopt(argc, argv, "f:o:")) != -1) {
			switch (opt) {
			case 'f':
				params->interface = optarg;
				break;
			case 'o':
				params->file = optarg;
				break;
			case '?':
				return ERR_UNKNOWN_ARGUMENT;
				break;
			}
		}
		// kontrola nactenych hodnot
		if (params->file.length() <= 0 || params->interface.length() <= 0) {
			return ERR_UNKNOWN_ARGUMENT;
		}

		return ERR_NO_ERROR;
	}

	/* Funkce nacte vstupni parametry pro scanner */
	int getScannerParameters(int argc, char **argv, t_scannerParams *params) {
		int errCode = ERR_NO_ERROR;
		int opt;

		// kontrola poctu vsupnich parametru
		if (argc != 5) {
			return ERR_ARGUMENT_COUNT;
		}
		
		while ((opt = getopt(argc, argv, "i:f:")) != -1) {
			switch (opt) {
			case 'i':
				params->interface = optarg;
				break;
			case 'f':
				params->file = optarg;
				break;
			case '?': 
				return ERR_UNKNOWN_ARGUMENT;
				break;
			}
		}
			// kontrola nactenych hodnot
		if (params->file.length() <= 0 || params->interface.length() <= 0) {
			return ERR_UNKNOWN_ARGUMENT;
		}

		return ERR_NO_ERROR;
	}

	/* Tisk chybove hlasky */
	void printError(int errCode) {
		switch (errCode) {
		case ERR_ARGUMENT_COUNT:
			std::cerr << "Spatny pocet vstupnich parametru!";
			break;
		case ERR_UNKNOWN_ARGUMENT:
			std::cerr << "Neznamy parametr!";
			break;
		case ERR_OUT_FILE:
			std::cerr << "Chyba pri otevirani vystupniho souboru!";
			break;
		case ERR_IN_FILE:
			std::cerr << "Chyba pri otevirani vstupniho souboru!";
			break;
		case ERR_ARP_SEND:
			std::cerr << "Chyba pri ziskavani IPv4 adres!";
			break;
		case ERR_IPV6:
			std::cerr << "Chyba pri ziskavani IPv6 adres!";
			break;
		case ERR_NO_IF_FOUND:
			std::cerr << "Nebylo nalezeno zadne odpovidajicci zarizeni!";
			break;
		case ERR_INIT:
			std::cerr << "Chyba pri inicializaci zarizeni!";
			break;
		case ERR_ARP_SPOOF:
			std::cerr << "Chyba pri pokusu o ARP spoof!";
			break;
		case ERR_NDP_SPOOF:
			std::cerr << "Chyba pri pokusu o NDP spoof!";
			break;
		case ERR_INTERCEPT:
			std::cerr << "Chyba pri pokusu o preposilani provozu!";
			break;
		default:
			std::cerr << "Neznama chyba!";
			break;
		}
		std::cerr << std::endl;
	}
}
/*
* Soubor:  Packets.h
* Datum:   2017/4/20
* Autor:   Bc. Lukas Pelanek, xpelan03@stud.fit.vutbr.cz
* Projekt: PDS, Man in the Middle
* Popis:   Soubor obsahujici struktury reprezentujici hlavicky paketu
*/

#ifndef PACKETS_H
#define PACKETS_H

#include <cstdint>

typedef struct _arp_hdr arp_hdr;
struct _arp_hdr {
	uint16_t htype;
	uint16_t ptype;
	uint8_t hlen;
	uint8_t plen;
	uint16_t opcode;
	uint8_t sender_mac[6];
	uint8_t sender_ip[4];
	uint8_t target_mac[6];
	uint8_t target_ip[4];
};

typedef struct _ethernet_header eth_hdr;
struct _ethernet_header {
	uint8_t dest[6];
	uint8_t source[6];
	uint16_t type;
};

typedef struct _ipv4_header ipv4_header;
struct _ipv4_header {

	uint8_t ip_header_len : 4;
	uint8_t ip_version : 4;

	uint8_t ip_tos;
	uint16_t ip_total_length;
	uint16_t ip_id; 
	uint8_t ip_frag_offset : 5;
	uint8_t ip_more_fragment : 1;
	uint8_t ip_dont_fragment : 1;
	uint8_t ip_reserved_zero : 1;
	uint8_t ip_frag_offset1;

	uint8_t ip_ttl;
	uint8_t ip_protocol;
	uint16_t ip_checksum;
	uint32_t ip_srcaddr; 
	uint32_t ip_destaddr; 
};

#endif
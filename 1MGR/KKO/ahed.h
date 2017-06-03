/*
 * Autor: Lukas pelanek (xpelan03)
 * Datum: 29.4.2017
 * Soubor: ahed.h
 * Komentar: Hlavickovy soubor knihovny
 */ 

#ifndef __KKO_AHED_H__
#define __KKO_AHED_H__

#include <stdio.h>
#include <string>
#include <sys/types.h>
#include <cstdlib>
#include <iostream>
#include <cstdio>
#include <stack>

#define AHEDOK 0
#define AHEDFail -1

#define NYT 256			    // NOT YET TRANSFERED
#define TOTAL_SYMBOLS 257	// celkovy pocet symbolu
#define NODE_SYMBOL 257		// Uzel

/* Datovy typ zaznamu o (de)kodovani */
typedef struct{
	/* velikost nekodovaneho retezce */
	int64_t uncodedSize;
	/* velikost kodovaneho retezce */
	int64_t codedSize;
} tAHED;

typedef struct t_Node_struct {
	uint64_t weight;
	uint16_t data;
	uint16_t index;

	struct t_Node_struct *parentNode;
	struct t_Node_struct *rightNode;
	struct t_Node_struct *leftNode;
} t_Node;

/* Rekurzivne odstrani strom */
void deleteTree(t_Node *tree);

// vytiskne kod uzlu
int outputCode(t_Node *node, uint8_t *buffer, uint8_t *bufferPosition, FILE *outputFile);

/* Vytiskne symbol a vrati pocet zapsanych bajtu */
int outputSymbol(uint8_t symbol, uint8_t *buffer, uint8_t *bufferPosition, FILE *outputFile);

/* Vlozi symbol do stromu a posune NYT stromem dolu */
int addSymbolIntoTree(t_Node ** symbols, uint16_t symbol);

/* Zkontroluje, zda strom je stale huffmanuv a inkrementuje vyskyt uzlu. Pokud ne, tak jej presklada */
void checkHuffmanTree(t_Node **symbols, t_Node *node);

/* Nazev:
 *   AHEDEncoding
 * Cinnost:
 *   Funkce koduje vstupni soubor do vystupniho souboru a porizuje zaznam o kodovani.
 * Parametry:
 *   ahed - zaznam o kodovani
 *   inputFile - vstupni soubor (nekodovany)
 *   outputFile - vystupní soubor (kodovany)
 * Navratova hodnota: 
 *    0 - kodovani probehlo v poradku
 *    -1 - pøi kodovani nastala chyba
 */
int AHEDEncoding(tAHED *ahed, FILE *inputFile, FILE *outputFile);


/* Nazev:
 *   AHEDDecoding
 * Cinnost:
 *   Funkce dekoduje vstupni soubor do vystupniho souboru a porizuje zaznam o dekodovani.
 * Parametry:
 *   ahed - zaznam o dekodovani
 *   inputFile - vstupni soubor (kodovany)
 *   outputFile - vystupní soubor (nekodovany)
 * Navratova hodnota: 
 *    0 - dekodovani probehlo v poradku
 *    -1 - pøi dekodovani nastala chyba
 */
int AHEDDecoding(tAHED *ahed, FILE *inputFile, FILE *outputFile);



#endif


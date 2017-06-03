/*
 * Autor: Lukas pelanek (xpelan03)
 * Datum: 29.4.2017
 * Soubor: ahed.cpp
 * Komentar: Knihovni funkce
 */ 

#include "ahed.h"

/* Rekurzivne odstrani strom */
void deleteTree(t_Node *tree) {
	if (tree) {
		deleteTree(tree->leftNode);
		deleteTree(tree->rightNode);
		free(tree);
	}
}

/* Zkontroluje, zda strom je stale huffmanuv a inkrementuje vyskyt uzlu. Pokud ne, tak jej presklada */
void checkHuffmanTree(t_Node **symbols, t_Node *node) {
	t_Node *actualNode = node;
	t_Node *lowIndexNode = NULL;
	t_Node *tmpNode = NULL;

	int x;
	uint16_t tmpIndex;

	while (actualNode != NULL)
	{
		// hledame uzel s nejmensim indexem
		lowIndexNode = actualNode;
		for (x = 0; x < 2 * TOTAL_SYMBOLS; x++) {
			if (symbols[x] != NULL && symbols[x]->weight == actualNode->weight && symbols[x]->index < lowIndexNode->index) {
				lowIndexNode = symbols[x];
			}
		}

		// prohodime uzly
		if (lowIndexNode != actualNode && lowIndexNode != actualNode->parentNode) {

			// ulozime pomocne promenne
			tmpNode = actualNode->parentNode;
			tmpIndex = actualNode->index;

			// prohodime synovske uzly
			if (actualNode->parentNode->leftNode == actualNode)
				actualNode->parentNode->leftNode = lowIndexNode;
			else
				actualNode->parentNode->rightNode = lowIndexNode;

			// prohodime rodicovske uzly druheho uzlu
			if (lowIndexNode->parentNode->leftNode == lowIndexNode)
				lowIndexNode->parentNode->leftNode = actualNode;
			else
				lowIndexNode->parentNode->rightNode = actualNode;

			// prohodime rodice
			actualNode->parentNode = lowIndexNode->parentNode;
			lowIndexNode->parentNode = tmpNode;

			// prohodime indexy
			actualNode->index = lowIndexNode->index;
			lowIndexNode->index = tmpIndex;
		}

		// zvysime vahu a posuneme se o uroven vyse
		actualNode->weight++;
		actualNode = actualNode->parentNode;
	}
}


/* Vlozi symbol do stromu a posune NYT stromem dolu */
int addSymbolIntoTree(t_Node ** symbols, uint16_t symbol) {
	t_Node *leftNode;
	t_Node *rightNode;

	// alokace pameti
	if ((leftNode = (t_Node*)malloc(sizeof(struct t_Node_struct))) == NULL) {
		std::cout << "Chyba pri volani malloc()" << std::endl;
		return AHEDFail;
	}
	else if ((rightNode = (t_Node*)malloc(sizeof(struct t_Node_struct))) == NULL) {
		free(leftNode);
		std::cout << "Chyba pri volani malloc()" << std::endl;
		return AHEDFail;
	}

	// nastaveni ukazatelu rodice
	rightNode->parentNode = symbols[NYT];
	leftNode->parentNode = symbols[NYT];

	// nastaveni ukazatelu synovskych uzlu
	rightNode->leftNode = NULL;
	rightNode->rightNode = NULL;
	leftNode->leftNode = NULL;
	leftNode->rightNode = NULL;

	// do praveho uzlu vlozime symbol
	rightNode->data = symbol;
	rightNode->weight = 0;
	rightNode->index = symbols[NYT]->index + 1;

	// do leveho uzlu vlozime NYT
	leftNode->data = NYT;
	leftNode->weight = 0;
	leftNode->index = symbols[NYT]->index + 2;

	// uzel NYT se stava rodicem novych uzlu, NYT se stava uzlem beze znaku
	symbols[NYT]->data = NODE_SYMBOL;
	symbols[NYT]->leftNode = leftNode;
	symbols[NYT]->rightNode = rightNode;

	// vlozeni uzlu do pole
	symbols[TOTAL_SYMBOLS + symbols[NYT]->index / 2] = symbols[NYT];
	symbols[NYT] = leftNode;
	symbols[symbol] = rightNode;

	return AHEDOK;
}

 /* Vytiskne symbol a vrati pocet zapsanych bajtu */
int outputSymbol(uint8_t symbol, uint8_t *buffer, uint8_t *bufferPosition, FILE *outputFile) {
	bool bitValue;
	int x;

	for (x = 0; x < 8; x++) {
		bitValue = (symbol >> x) & 1;		// hodnota bitu na x-te pozici
		*buffer |= bitValue << *bufferPosition;	// nastaveni bitu v bufferu
		(*bufferPosition)++;

		// kdyz je cely buffer zaplnen, tak jej vytiskneme do souboru a inkrementujeme pocet zapsanych bajtu
		if (*bufferPosition == 8) {
			std::fwrite(buffer, 1, 1, outputFile);

			// vyprazdnime buffer a vynulujeme pozici
			*buffer = 0;
			*bufferPosition = 0;
		}
	}
	return 1;	// vzdy zapiseme jen 1 bajt
}

/* Vytiskne kod uzlu a vrati pocet zapsanych bajtu */
int outputCode(t_Node *node, uint8_t *buffer, uint8_t *bufferPosition, FILE *outputFile) {
	int bytesCount = 0;
	std::stack<bool> path;		// zasobnik pro ulozeni cesty
	t_Node *tmp = node;

	// projdu stromem az ke koreni a ulozim cestu na zasobnik
	while (tmp->parentNode != NULL) {
		path.push(tmp == tmp->parentNode->rightNode);
		tmp = tmp->parentNode;
	}

	// vypisi cestu
	while (!path.empty()) {
		*buffer |= path.top() << *bufferPosition;	// nastavim bit
		(*bufferPosition)++;							// inkrementuji pozici v bufferu

		// kdyz je cely buffer zaplnen, tak jej vytiskneme do souboru a inkrementujeme pocet zapsanych bajtu
		if (*bufferPosition == 8) {
			std::fwrite(buffer, 1, 1, outputFile);

			// vyprazdnime buffer a vynulujeme pozici
			*buffer = 0;
			*bufferPosition = 0;
			bytesCount++;
		}
		path.pop();		// vyberu dalsi prvek ze zasobniku
	}
	return bytesCount;
}

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
int AHEDEncoding(tAHED *ahed, FILE *inputFile, FILE *outputFile) {
	int x;
	// korenovy uzel
	t_Node *rootNode;
	// kazdy symbol bude ulozen v poli pro snadnejsi pristup
	t_Node *symbols[TOTAL_SYMBOLS * 2];

	uint8_t input;
	uint8_t buffer = 0;
	uint8_t bufferPosition = 0;

	ahed->codedSize = 0;
	ahed->uncodedSize = 0;
	
	// alokace korenoveho uzlu
	if ((rootNode = (t_Node *)malloc(sizeof(struct t_Node_struct))) == NULL) {
		std::cerr << "Chyba pri volani malloc()" << std::endl;
		return AHEDFail;
	}

	// inicializace ukazatelu na symboly
	for (x = 0; x < TOTAL_SYMBOLS * 2; x++) {
		symbols[x] = NULL;
	}

	// inicializace korenoveho uzlu, vlozeni znaku NYT
	rootNode->data = NYT;
	rootNode->index = 0;
	rootNode->weight = 0;
	rootNode->leftNode = NULL;
	rootNode->rightNode = NULL;
	rootNode->parentNode = NULL;

	// vlozeni NYT do pole znaku
	symbols[NYT] = rootNode;

	while (1) {
		std::fread(&input, 1, 1, inputFile);
		if (feof(inputFile) || ferror(inputFile))  break;

		ahed->uncodedSize++;

		// prvni nacteni
		if (symbols[input] == NULL) {

			// tisk kodu uzlu NYT
			ahed->codedSize += outputCode(symbols[NYT], &buffer, &bufferPosition, outputFile);

			// tisk znaku
			ahed->codedSize += outputSymbol(input, &buffer, &bufferPosition, outputFile);

			// vlozi symbol do stromu
			if (addSymbolIntoTree(symbols, input) != AHEDOK) {
				deleteTree(rootNode);
				return AHEDFail;
			}

		}
		// jinak vytiskni kod znaku
		else {
			ahed->codedSize += outputCode(symbols[input], &buffer, &bufferPosition, outputFile);
		}

		// zkontroluj, zda strom je stale huffmanuv
		checkHuffmanTree(symbols, symbols[input]);
	}

	// tisk NYT jako konec souboru
	ahed->codedSize += outputCode(symbols[NYT], &buffer, &bufferPosition, outputFile);

	// vytisknu zbytek bufferu
	if (bufferPosition != 0) {
		std::fwrite(&buffer, 1, 1, outputFile);
		ahed->codedSize++;
	}

	// zrus strom
	deleteTree(rootNode);

	return AHEDOK;
}

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
int AHEDDecoding(tAHED *ahed, FILE *inputFile, FILE *outputFile) {
	int x;
	// korenovy uzel
	t_Node *rootNode;
	// kazdy symbol bude ulozen v poli pro snadnejsi pristup
	t_Node *symbols[TOTAL_SYMBOLS * 2];

	// pomocny ukazatel na uzel
	t_Node *node;

	uint8_t input;
	uint8_t buffer = 0;
	uint8_t bufferPosition = 0;

	ahed->codedSize = 0;
	ahed->uncodedSize = 0;

	// alokace korenoveho uzlu
	if ((rootNode = (t_Node *)malloc(sizeof(struct t_Node_struct))) == NULL) {
		std::cerr << "Chyba pri volani malloc()" << std::endl;
		return AHEDFail;
	}

	// inicializace ukazatelu na symboly
	for (x = 0; x < TOTAL_SYMBOLS * 2; x++) {
		symbols[x] = NULL;
	}

	// inicializace korenoveho uzlu, vlozeni znaku NYT
	rootNode->data = NYT;
	rootNode->index = 0;
	rootNode->weight = 0;
	rootNode->leftNode = NULL;
	rootNode->rightNode = NULL;
	rootNode->parentNode = NULL;

	// vlozeni NYT do pole znaku
	symbols[NYT] = rootNode;
	node = rootNode;

	bool bitValue;
	bool isNYT = true;

	while (1) {
		std::fread(&input, 1, 1, inputFile);
		if (feof(inputFile) || ferror(inputFile)) {
			break;
		}

		// provedeme jednotlive po bitech
		for (x = 0; x < 8; x++) {

			// precteme hodnotu bitu
			bitValue = (input >> x) & 1;

			if (isNYT) {
				// ulozime do bufferu
				buffer |= bitValue << bufferPosition;
				bufferPosition++;

				// pokud jsme zaplnili buffer, tak vytiskneme znak na vystup
				if (bufferPosition == 8) {

					// dalsi znak nebude NYT
					isNYT = false;

					// tisk
					std::fwrite(&buffer, 1, 1, outputFile);

					// nasledne vloizime znak do stromu a zkontrolujeme, zda se jedna o huffmanuv strom
					if (addSymbolIntoTree(symbols, buffer) != AHEDOK) {
						deleteTree(rootNode);
						return AHEDFail;
					}

					// kontrola, zda se stale jedna o huffmanuv strom
					checkHuffmanTree(symbols, symbols[buffer]);

					// nastaveni hodnot bufferu a velikost dat
					buffer = 0;
					bufferPosition = 0;
					ahed->uncodedSize++;
				}
			}
			// jinak prochazime stromem
			else {
				// pokud hodnota bitu je 1, prochazime stromem vpravo
				if (bitValue == 1 && node->rightNode != NULL) {
					node = node->rightNode;
				}
				// jinak vlevo
				else if (bitValue == 0 && node->leftNode != NULL) {
					node = node->leftNode;
				}
				// jinak chyba
				else {
					deleteTree(rootNode);
					return AHEDFail;
				}

				// dalsi znak nactu cely
				if (node->data == NYT) {
					node = rootNode;
					isNYT = true;
				}
				// dekodovali jsme symbol a vypiseme jej 
				else if (node->data < NYT) {
					std::fwrite(&node->data, 1, 1, outputFile);

					// zkontrolujeme, zda se stale jedna o huffmanuv strom
					checkHuffmanTree(symbols, node);
					node = rootNode;
					ahed->uncodedSize++;
				}
			}
		}
		// rozkodovali jsme dalsi symbol
		ahed->codedSize++;
	}

	// uvolnime strom
	deleteTree(rootNode);

	// posledni znak musi byt NYT
	if (!isNYT) {
		return AHEDFail;
	}

	return AHEDOK;
}
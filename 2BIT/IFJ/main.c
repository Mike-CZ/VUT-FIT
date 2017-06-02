/*
* Soubor:  main.c
* Datum:   2014/12/14
* Autori:  Lukas Pelanek, xpelan03@stud.fit.vutbr.cz
*		   Hana Prostrednikova, xprost01@stud.fit.vutbr.cz
*		   Zuzana Skalnikova, xskaln04@stud.fit.vutbr.cz
*		   Vitezslav Skrivanek, xskriv11@stud.fit.vutbr.cz
* Projekt: Interpret jazyka IFJ14, projekt do predmetu IFJ
* Popis:   Program nacte zdrojovy soubor zapsany v jazyce IFJ14 a interpretuje jej.
*/
#include "scanner.h"
#include "garbage_collector.h"
#include "parser.h"
#include "ial.h"
#include "precedence.h"
#include "ilist.h"
#include <stdlib.h>

	FILE *file;		// soubor globalne

int main(int argc, char** argv){

	int result = SYNTAX_OK;

	if (argc != 2){
		return 99;
	}


	if ((file = fopen(argv[1], "r")) == NULL){
		return INT_ERR;
	}

    symbolTablePtr symbolTable;
    BTInit(&symbolTable);

    result = parse(&symbolTable);

	gFree();

	fclose(file);

	return result;
}

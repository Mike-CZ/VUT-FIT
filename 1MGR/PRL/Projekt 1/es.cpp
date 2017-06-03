/*
* Soubor:  es.cpp
* Datum:   2017/1/4
* Autor:   Bc. Lukas Pelanek, xpelan03@stud.fit.vutbr.cz
* Projekt: PRL, Enumeration sort
* Popis:   Program nacte vstupni soubor s nazvem "numbers", ktery obsahuje neserazenou posloupnost cisel a pomoci paralelniho algoritmu Enumeration sort tuto posloupnost seradi a vypise na stdout.
*/

#include <mpi.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <fstream>

#define TAG 0

#define FILENAME "numbers"

/*
*	Kostra prevzata z ukazky "hello.c", ktera je ke stazeni ve wis
*/

int main(int argc, char **argv) {

	std::vector<int> numbers;					
	int numprocs;
	int myid;
	MPI_Status stat;

	int c, x, y, z, h, k;
	bool checkX, checkY;
	int tmpC;
	int zCount, zVal;

	MPI_Init(&argc, &argv); 
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs); 
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);

	numprocs -= 1;										// nepocitame ridici procesor
							  
	// prvni procesor nacte vstupni data a rozesle hodnoty dalsim procesorum
	if (myid == 0) {
		int number;

		std::ifstream fin(FILENAME, std::fstream::in);  // otevru soubor                                  

		while (fin.good()) {
			number = fin.get();							// nacti cislo

			if (!fin.good()) break;						// pokud eof skonci

			numbers.push_back(number);				// uloz do vectoru
		}

		// tisk nactenych hodnot
		for (k = 0; k < numbers.size(); k++) {
			std::cout << numbers[k];
			if (k < numbers.size() - 1) {
				std::cout << " ";
			}
			else {
				std::cout << std::endl;
			}
		}

		fin.close();									// zavri stream
	}

	checkX = false;
	checkY = false;

	// zacatek algoritmu podle prednasky prl

	c = 1;												// registr C nastavime na hodnotu 1
	zCount = 0;											// pomocny registr pro razeni stejnych hodnot

	for (k = 1; k <= (2 * numprocs); k++) {

		h = (k <= numprocs) ? 1 : k - numprocs;

		c = ((checkX && checkY) && x > y && myid >= h) ? c + 1 : c;

		// shift registru Y, posledni registr se neposouva
		if (myid >= h && myid < numprocs && checkY) {
			MPI_Send(&y, 1, MPI_INT, myid + 1, TAG, MPI_COMM_WORLD);
		}
		if (myid >= (h + 1) && myid <= k && myid <= numprocs) {
			MPI_Recv(&y, 1, MPI_INT, myid - 1, TAG, MPI_COMM_WORLD, &stat);
			checkY = true;
		}

		// pokud vstup neni vycerpan, vstupni prvek xi se vlozi Xi a do Y1
		if (myid == 0 && (k <= numprocs)) {
			MPI_Send(&numbers[k - 1], 1, MPI_INT, 1, TAG, MPI_COMM_WORLD);
			MPI_Send(&numbers[k - 1], 1, MPI_INT, k, TAG, MPI_COMM_WORLD);
		}
		if (myid == k && (k <= numprocs)) {
			MPI_Recv(&x, 1, MPI_INT, 0, TAG, MPI_COMM_WORLD, &stat);
			checkX = true;
		}
		if (myid == 1 && (k <= numprocs)) {
			MPI_Recv(&y, 1, MPI_INT, 0, TAG, MPI_COMM_WORLD, &stat);
			checkY = true;
		}

		if (k > numprocs) {
			tmpC = c;	

			// odesleme broadcastem prijemce a ulozime prijemce do pomocne promenne (aby se nam neprepsal prijemce pro dalsi krok)
			MPI_Bcast(&tmpC, 1, MPI_INT, h, MPI_COMM_WORLD);

			// prislusny procesor odpovi zaslanim sveho registru zcount a inkrementuje jeho obsah
			if (myid == tmpC) {
				MPI_Send(&zCount, 1, MPI_INT, h, TAG, MPI_COMM_WORLD);
				zCount++;
			}

			// procesor ktery odesila vysledek obdrzi obsah registru zcount
			if (myid == h) {
				MPI_Recv(&zVal, 1, MPI_INT, tmpC, TAG, MPI_COMM_WORLD, &stat);
				tmpC += zVal;
			}

			// pomoci broadcastu informujeme noveho prijemce
			MPI_Bcast(&tmpC, 1, MPI_INT, h, MPI_COMM_WORLD);

			// odesleme obsah registru novemu procesoru
			if (myid == h) {
				MPI_Send(&x, 1, MPI_INT, tmpC, TAG, MPI_COMM_WORLD);
			}

			// novy procesor prijme obsah
			if (myid == tmpC) {
				MPI_Recv(&z, 1, MPI_INT, h, TAG, MPI_COMM_WORLD, &stat);

			}
		}
	}

	for (k = 1; k <= numprocs; k++) {

		// zasleme obsah registru vedlejsimu procesoru (posledni procesor posila data ridicimu procesoru)
		MPI_Send(&z, 1, MPI_INT, (myid + 1) % (numprocs + 1) , TAG, MPI_COMM_WORLD);

		// ridici procesor uklada nova data 
		if (myid == 0) {
			MPI_Recv(&numbers[k - 1], 1, MPI_INT, numprocs, TAG, MPI_COMM_WORLD, &stat);
		}
		// ostatni procesory posunuji obsah registru
		else if (myid > 1) {
			MPI_Recv(&z, 1, MPI_INT, myid - 1, TAG, MPI_COMM_WORLD, &stat);
		}
	}

	// ridici procesor vypise serazenou posloupnost
	if (myid == 0) {
		// vypis od konce, protoze posledni procesor generoval serazenou posloupnost od nejvetsiho po nejmensi (proto na indexu 0 nejvyssi cislo)
		for (k = numbers.size() - 1; k >= 0; k--) {
			std::cout << numbers[k] << std::endl;
		}
	}

	MPI_Finalize();
	return 0;
}

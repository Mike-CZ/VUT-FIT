/*
* Soubor:  mm.cpp
* Datum:   2017/7/4
* Autor:   Bc. Lukas Pelanek, xpelan03@stud.fit.vutbr.cz
* Projekt: PRL, Mesh Multiplication
* Popis:   Vstupem jsou textove soubory mat1 a mat2. Program nacte vstupni soubory, ktere obsahuji matice a pomoci algoritmu Mesh Multiplication je vynasobi.
*/

#include <mpi.h>
#include <cstdlib>
#include <string>
#include <vector>
#include <fstream>

#define TAG 0

#define FILE1 "mat1"
#define FILE2 "mat2"

enum T_ERR {
	NO_ERROR = 0,
	NOT_NUMBER = 1,
	WRONG_SIZE = 2,
	WRONG_COUNT = 3,
	WONG_ROWS_COLS_SIZE = 4
};

/*
*	Kostra prevzata z ukazky "hello.c", ktera je ke stazeni ve wis
*/

int main(int argc, char **argv) {
	int numprocs, error, myid, Cij, tmpVal, x, a, b;
	MPI_Status stat;
	int m1_rowsCount = -1;
	int m1_colsCount = -1;
	int m2_rowsCount = -1;
	int m2_colsCount = -1;
	std::vector<int> inputNumbers;

	int matrix_size[4];

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);

	error = NO_ERROR;

	// prvni procesor nacte vstupni data
	if (myid == 0) {
		std::string str;
		std::vector<int> m1_nums;
		std::vector<int> m2_nums;
		int number;
		char *test;

		std::ifstream fin(FILE1, std::fstream::in);			// otevru prvni soubor
		std::ifstream fin2(FILE2, std::fstream::in);		// otevru druhy soubor

		while (fin >> str) {
			number = strtol(str.c_str(), &test, 10);

			if (*test) {
				error = NOT_NUMBER;
				break;
			}
			else {
				if (m1_rowsCount == -1) {					// ziskame pocet radku prvni matice
					if (number < 1) {						// error, matice musi mit alespon 1 radek
						error = WRONG_SIZE;
						break;
					}
					m1_rowsCount = number;					// prvni nactene cislo je pocet radku
				}
				else {
					m1_nums.push_back(number);
				}
			}
		}
		fin.close();

		// zpracovani druheho souboru (pokud v prvnim nenastala chyba)
		if (!error) {
			while (fin2 >> str) {
				number = strtol(str.c_str(), &test, 10);

				if (*test) {
					error = NOT_NUMBER;
					break;
				}
				else {
					if (m2_colsCount == -1) {					// ziskame pocet sloupcu prvni matice
						if (number < 1) {						// error, matice musi mit alespon 1 sloupec
							error = WRONG_SIZE;
							break;
						}
						m2_colsCount = number;					// prvni nactene cislo je pocet sloupcu
					}
					else {
						m2_nums.push_back(number);
					}
				}
			}
		}
		fin2.close();

		if (!error) {
			// kontrola vstupni matice a vypocet radku/sloupcu
			if (!(m1_nums.size() > 0 && (m1_nums.size() / (double)m1_rowsCount) - (int)(m1_nums.size() / m1_rowsCount) == 0) ||
				!(m2_nums.size() > 0 && (m2_nums.size() / (double)m2_colsCount) - (int)(m2_nums.size() / m2_colsCount) == 0)) {
				error = WRONG_COUNT;
			}
			else {
				m1_colsCount = m1_nums.size() / m1_rowsCount;
				m2_rowsCount = m2_nums.size() / m2_colsCount;
			}

			// pocet sloupcu prvni matice a pocet radku druhe matice se musi rovnat, jinak nemuzeme matice nasobit!
			if (m1_colsCount != m2_rowsCount) {
				error = WONG_ROWS_COLS_SIZE;
			}
		}

		// pokud nenastala chyba pri zpracovani vstupniho souboru, tak zacnu rozesilat data
		if (!error) {
			// ulozim rozmery matice do pole
			matrix_size[0] = m1_rowsCount;
			matrix_size[1] = m1_colsCount;
			matrix_size[2] = m2_rowsCount;
			matrix_size[3] = m2_colsCount;

			int x, x2, y, processor, loops, syncA, syncB, send;
			x = x2 = processor = syncA = syncB = send = 0;
			bool startRow = false;

			// iterujeme podle vetsi matice (mensi se stihne odeslat)
			loops = (m1_nums.size() > m2_nums.size()) ? m1_nums.size() : m2_nums.size();

			// odesleme vsechny prvky synchronne, vzdy se zahaji odeslani noveho sloupce/radku pokud byly oba odeslany (aby proceor 0 dostaval prvky ve spravnem poradi)
			for (y = 0; y < loops; y++) {
				// nejprve odesleme radkovy prvek
				if (x < m1_nums.size() && syncA <= syncB) {
					MPI_Send(&m1_nums[send], 1, MPI_INT, processor, TAG, MPI_COMM_WORLD);

					// pokud dojedeme na posledni radek, inkrementujeme synchronizacni promennou
					if ((x + 1) % (m1_rowsCount) == 0) {
						syncA++;
						send = syncA;
						startRow = true;
					}
					else {
						send += m1_colsCount;
						startRow = false;
					}

					processor = (processor + m2_colsCount) % (m1_rowsCount * m2_colsCount);
					x++;
				}

				// osetreni aby se vzdy vkladal radek jako prvni
				if (startRow && syncA == syncB) {
					continue;
				}

				// pote odesleme sloupcovy prvek
				if (x2 < m2_nums.size() && syncB <= syncA) {
					MPI_Send(&m2_nums[x2], 1, MPI_INT, x2 % m2_colsCount, TAG, MPI_COMM_WORLD);

					// pokud dojdeme na posledni sloupec, inkrementujeme synchronizacni promennou
					if ((x2 + 1) % m2_colsCount == 0) {
						syncB++;
					}

					x2++;
				}
			}
		}
	}

	// synchronizace procesoru, abychom je mohli pripadne ukoncit pokud nastala chyba pri nacitani vsutpnich souboru
	MPI_Barrier(MPI_COMM_WORLD);

	// ostatnim procesorum zasleme zpravu, zda nastala chyba
	MPI_Bcast(&error, 1, MPI_INT, 0, MPI_COMM_WORLD);

	if (error) {
		if (myid == 0) {
			switch (error) {
			case NOT_NUMBER:
				std::cout << "Vstupni matice musi obsahovat pouze cela cisla!" << std::endl;
				break;
			case WRONG_SIZE:
				std::cout << "Pocet sloupcu/radku musi byt alespon 1!" << std::endl;
				break;
			case WRONG_COUNT:
				std::cout << "Nesouhlasi pocet radku a sloupcu vstupnich matic!" << std::endl;
				break;
			case WONG_ROWS_COLS_SIZE:
				std::cout << "Matice nelze vynasobit. Pocet sloupcu prvni matice musi byt stejny jako pocet radku druhe matice!" << std::endl;
				break;
			default:
				std::cout << "Doslo k nezname chybe!" << std::endl;
				break;
			}
		}
		MPI_Finalize();
		exit(-1);
	}

	// odeslu velikost matic ostatnim procesorum a ulozim
	MPI_Bcast(&matrix_size, 4, MPI_INT, 0, MPI_COMM_WORLD);
	m1_rowsCount = matrix_size[0];
	m1_colsCount = matrix_size[1];
	m2_rowsCount = matrix_size[2];
	m2_colsCount = matrix_size[3];

	// okrajove procesory ulozi vstupy do vectoru
	if (myid < m2_colsCount || myid % m2_colsCount == 0) {
		for (x = 0; x < m1_colsCount; x++) {
			// prvni procesor nacita data zleva i zvrchu
			if (myid == 0) {
				MPI_Recv(&tmpVal, 1, MPI_INT, 0, TAG, MPI_COMM_WORLD, &stat);
				inputNumbers.push_back(tmpVal);
			}
			MPI_Recv(&tmpVal, 1, MPI_INT, 0, TAG, MPI_COMM_WORLD, &stat);
			inputNumbers.push_back(tmpVal);
		}
	}

	Cij = 0;

	for (x = 0; x < m1_colsCount; x++) {
		// prvni procesor cte data z vectoru (radky a sloupce jsou ulozeny stridave a protoze je nasobeni komutativni nezalezi na poradi)
		if (myid == 0) {
			a = inputNumbers[x * 2];			// vector procesoru 0 je 2x delsi (obsahuje prvky a i b, proto nasobime index 2)
			b = inputNumbers[x * 2 + 1];
		}
		// sloupcove procesory ctou hodnotu b z vectoru a hodnotu a od vedlejsiho procesoru
		else if (myid < m2_colsCount) {
			MPI_Recv(&a, 1, MPI_INT, myid - 1, TAG, MPI_COMM_WORLD, &stat);
			b = inputNumbers[x];
		}
		// radkove procesory ctou hodnotu a z vectoru a hodnotu b od vrchnoho procesoru
		else if (myid % m2_colsCount == 0) {
			a = inputNumbers[x];
			MPI_Recv(&b, 1, MPI_INT, myid - m2_colsCount, TAG, MPI_COMM_WORLD, &stat);
		}
		// jinak prijmi od sousednich procesoru
		else {
			MPI_Recv(&a, 1, MPI_INT, myid - 1, TAG, MPI_COMM_WORLD, &stat);
			MPI_Recv(&b, 1, MPI_INT, myid - m2_colsCount, TAG, MPI_COMM_WORLD, &stat);
		}

		// pricti hodnotu C
		Cij += (a * b);

		// posli a procesorum napravo
		if ((myid + 1) % m2_colsCount != 0) {
			MPI_Send(&a, 1, MPI_INT, myid + 1, TAG, MPI_COMM_WORLD);
		}

		// posli b spodnim procesorum
		if (myid < m2_colsCount * (m1_rowsCount - 1)) {
			MPI_Send(&b, 1, MPI_INT, myid + m2_colsCount, TAG, MPI_COMM_WORLD);
		}
	}

	// odesleme vysledek prvnimu procesoru
	if (myid != 0) {
		MPI_Send(&Cij, 1, MPI_INT, 0, TAG, MPI_COMM_WORLD);
	}
	else {
		inputNumbers.clear();			// vyprazdnime vector a ulozime do nej vysledek
		inputNumbers.push_back(Cij);	// procesor 0 vlozi svuj vysledek

		for (x = 1; x < m1_rowsCount * m2_colsCount; x++) {
			MPI_Recv(&tmpVal, 1, MPI_INT, x, TAG, MPI_COMM_WORLD, &stat);
			inputNumbers.push_back(tmpVal);
		}

		// vytiskneme vyslednou matici
		std::cout << m1_rowsCount << ":" << m2_colsCount << std::endl;
		for (x = 0; x < inputNumbers.size(); x++) {
			std::cout << inputNumbers[x];
			if ((x + 1) % m2_colsCount == 0) {
				std::cout << std::endl;
			}
			else {
				std::cout << " ";
			}
		}
	}

	MPI_Finalize();
	return 0;
}

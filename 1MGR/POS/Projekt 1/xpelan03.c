/*
* Soubor:  xpelan03.c
* Datum:   2017/4/3
* Autor:   Bc. Lukas Pelanek, xpelan03@stud.fit.vutbr.cz
* Projekt: Ticket Algoritmus, projekt c. 1 pro predmet POS
* Popis:   Program po spusteni vytvori urcity pocet vlaken a pomoci operaci serializatoru se budou tato vlakna vzajemne synchronizovat.
*/

#define _XOPEN_SOURCE 500
#define _XOPEN_SOURCE_EXTENDED 1

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int ticket_number;
int cycles_number;
pthread_mutex_t ticketMutex;
pthread_cond_t condition;

/**
* @brief   Funkce vytiskne napovedu
* @param   void
*/
void printHelp() {
	printf("Program ocekava na prikazove radce jako prvni parametr pocet vlaken M a pocet pruchodu kritickou sekci N. N, M >= 0\nPriklad spusteni: program 1024 100\n");
}

/**
* @brief   Funkce vrati unikatni cislo listku, ktery urcuje poradi vstupu do kriticke sekce
* @param   void
*/
int getticket(void) {
	static int ticket = 0;
	int tmp_ticket;

	/* kriticka sekce */
	pthread_mutex_lock(&ticketMutex);
	tmp_ticket = ticket;
	++ticket;
	pthread_mutex_unlock(&ticketMutex);

	return tmp_ticket;
}

/**
* @brief   Funkce implementujici vstup do kriticke sekce
* @param   aenter Cislo prideleneho listku funkci getticket()
*/
void await(int aenter) {
	pthread_mutex_lock(&ticketMutex);
	while (aenter != ticket_number) {
		pthread_cond_wait(&condition, &ticketMutex);
	}
}

/**
* @brief   Funkce implementujici vystup z kriticke sekce
* @param   void
*/
void advance(void) {
	ticket_number++;
	pthread_mutex_unlock(&ticketMutex);
	pthread_cond_broadcast(&condition);
}

/**
* @brief   Funkce implementujici chovani vlakna
* @param   void
*/
void *TaskCode(void *id) {
	int ticket;
	long threadID;
	unsigned int seed;

	/* inicizalizace */
	threadID = (long)id;
	seed = (long)id;

	while ((ticket = getticket()) < cycles_number) { /* Pøidìlení lístku */
		/* Náhodné èekání v intervalu <0,0 s, 0,5 s> */
		nanosleep((const struct timespec[]) { {0, (rand_r(&seed) % 5001) * 100000} }, NULL);

		await(ticket);              /* Vstup do KS */
		printf("%d\t(%ld)\n", ticket, threadID + 1);
		fflush(stdout);
		advance();              /* Výstup z KS */

		/* Náhodné èekání v intervalu <0,0 s, 0,5 s> */
		nanosleep((const struct timespec[]) { {0, (rand_r(&seed) % 5001) * 100000} }, NULL);
	}

	pthread_exit(NULL);
}


int main(int argc, char *argv[]) {
	pthread_t *threads;
	pthread_attr_t attr;
	long *taskIDs;
	int rc, thread_number, threads_count;
	char *endptr;

	if (argc == 1) {
		printHelp();
		return 0;
	}
	else if (argc != 3) {
		printf("Chyba pri zadavani vstupnich parametru.\nSpustte aplikaci bez parametru pro spusteni napovedy.\n");
		return -1;
	}

	threads_count = (int)strtol(argv[1], &endptr, 10);
	if (!*argv[1] || *endptr || threads_count < 0) {
		printf("Chyba pri zadavani vstupnich parametru.\nSpustte aplikaci bez parametru pro spusteni napovedy.\n");
		return -1;
	}

	cycles_number = (int)strtol(argv[2], &endptr, 10);
	if (!*argv[2] || *endptr || cycles_number < 0) {
		printf("Chyba pri zadavani vstupnich parametru.\nSpustte aplikaci bez parametru pro spusteni napovedy.\n");
		return -1;
	}
	
	/* inicializace */
	ticket_number = 0;
	threads = (pthread_t*)malloc(threads_count * sizeof(pthread_t));
	taskIDs = (long*)malloc(threads_count * sizeof(long));
	pthread_attr_init(&attr); 
	pthread_mutex_init(&ticketMutex, NULL);
	pthread_cond_init(&condition, NULL);

	/* nastav vlaknum atribut JOINABLE */
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	/* vytvoreni vlaken */
	for (thread_number = 0; thread_number < threads_count; thread_number++) {

		taskIDs[thread_number] = thread_number;
		rc = pthread_create(&threads[thread_number], &attr, TaskCode, (void*)taskIDs[thread_number]);

		if (rc) {
			printf("Chyba s kodem \"%d\" pri volani funkce pthread_create()\n", rc);
			exit(-1);
		}
	}

	/* uvolni atributy */
	pthread_attr_destroy(&attr);

	/* cekani na dokonceni vlaken */
	for (thread_number = 0; thread_number < threads_count; thread_number++) {

		rc = pthread_join(threads[thread_number], NULL);

		if (rc) {
			printf("Chyba s kodem \"%d\" pri volani funkce pthread_join()\n", rc);
			exit(-1);
		}
	}

	free(threads);
	free(taskIDs);

	pthread_cond_destroy(&condition);
	pthread_mutex_destroy(&ticketMutex);

	return 0;
}
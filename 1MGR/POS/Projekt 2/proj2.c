#define _POSIX_SOURCE
/*
* Soubor:  proj2.c
* Datum:   2017/4/23
* Autor:   Bc. Lukas Pelanek, xpelan03@stud.fit.vutbr.cz
* Projekt: Shell, projekt c. 2 pro predmet POS
* Popis:   Jednoduchy shell, ktery podporuje nasledujici funkce: <, >, &
*/

#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>

#define ARGS_BUFF_SIZE 64
#define INPUT_BUFF_SIZE 513

int errFlag;				// kladne honoty -> korektni ukonceni, zaporne hodnoty -> chyba programu
pthread_mutex_t mutex;
pthread_cond_t condition;
char input[513];
int foregroundProcessId;

/* Signalizace druhemu vlaknu */
void Signalize() {
	pthread_mutex_lock(&mutex);
	pthread_cond_signal(&condition);
	pthread_mutex_unlock(&mutex);
}

/* Ceka na dokonceni druheho vlakan */
void Wait() {
	pthread_mutex_lock(&mutex);
	pthread_cond_wait(&condition, &mutex);
	pthread_mutex_unlock(&mutex);
}

/* Parsovani vstupu */
char **parseLine(char *line, int *background, char *inFile, char *outFile) {
	int index = 0;
	int bufferLength = ARGS_BUFF_SIZE;
	char *item;
	char **arguments = malloc(ARGS_BUFF_SIZE * sizeof(char*));

	if (arguments == NULL) {
		fprintf(stderr, "Chyba pri volani malloc()\n");
		exit(EXIT_FAILURE);
	}

	item = strtok(line, " \t\r\n\a");

	while (item != NULL) {
		// zpracovani specialnich znaku
		if (strlen(item) == 1) {
			if (*item == '&' && *background <= 0) {
				*background = index;
			}
			else if (*item == '<') {
				if ((item = strtok(NULL, " \t\r\n\a")) == NULL) {
					inFile[0] = '\0';
				}
				else {
					strcpy(inFile, item);
				}
			}
			else if (*item == '>') {
				if ((item = strtok(NULL, " \t\r\n\a")) == NULL) {
					outFile[0] = '\0';
				}
				else {
					strcpy(outFile, item);
				}
			}
			arguments[index] = NULL;
			return arguments;
		}

		// vlozeni argumentu
		arguments[index] = item;
		index++;
		if (index >= bufferLength) {
			bufferLength += ARGS_BUFF_SIZE;
			if ((arguments = realloc(arguments, bufferLength * sizeof(char *))) == NULL) {
				fprintf(stderr, "Chyba pri volani realloc()\n");
				exit(EXIT_FAILURE);
			}
		}
		item = strtok(NULL, " \t\r\n\a");
	}

	arguments[index] = NULL;
	return arguments;
}

/* Hlavni telo vlakna, ktere spousti programy */
void *cmdThreadHandler() {
	char **args;
	int onBackground;
	char inFile[INPUT_BUFF_SIZE];
	char outFile[INPUT_BUFF_SIZE];
	pid_t pid;
	int status;
	int fd;

	while (errFlag == 0) {
		Wait();
		onBackground = -1;
		inFile[0] = '\0';
		outFile[0] = '\0';

		// nacti parametry
		args = parseLine(input, &onBackground, inFile, outFile);

		// test zda nenastala chyba pri cteni
		if (errFlag != 0) {
			return NULL;
		}

		// spoustime pouze s argumenty po znak &
		if (onBackground > 0) {
			args[onBackground] = NULL;
		}

		pid = fork();
		if (pid == 0) {

			// otevreni souboru pro zapis
			if(*outFile != '\0') {
				if ((fd = open(outFile, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR)) < 0) {
					perror("Nepodarilo se otevrit vystupni soubor");
					exit(EXIT_FAILURE);
				}
				dup2(fd, 1);
				close(fd);
			}
			// otevreni souboru pro cteni
			else if (*inFile != '\0') {
				if ((fd = open(inFile, O_RDONLY)) < 0) {
					perror("Nepodarilo se otevrit vstupni soubor");
					exit(EXIT_FAILURE);
				}
				dup2(fd, 0);
				close(fd);
			}

			// spusteni programu
			if (execvp(args[0], args) == -1) {
				perror("Chyba pri spousteni procesu");
				// pokud chybu vyhodil proces na pozadi, vytisknu novy terminal
				if (onBackground > 0) {
					printf("$ ");
					fflush(stdout);
				}
				exit(EXIT_FAILURE);
			}
		}
		else if (pid < 0) {
			free(args);
			fprintf(stderr, "Chyba pri volani fork()\n");
			exit(EXIT_FAILURE);
		}
		// Parent process
		else {
			// cekame na synovsky proces pokud nema bezet na popredi
			if (onBackground <= 0) {
				pthread_mutex_lock(&mutex);
				foregroundProcessId = pid;				// bezici program na popredi id
				pthread_mutex_unlock(&mutex);

				waitpid(pid, &status, 0);				// cekame na dokonceni synovskeho procesu

				pthread_mutex_lock(&mutex);
				foregroundProcessId = 0;				// na popredi shell
				pthread_mutex_unlock(&mutex);
			}
		}

		free(args);

		// signalizujeme, ze ocekavame dalsi prikazy
		Signalize();
	}

	return NULL;

}

/* Hlavni telo vlakna, ktere obsluhuje uzivatele */
void *readThreadHandler() {
	int length = 0;
	int ch;
	while (errFlag == 0) {

		// inicializace 
		memset(input, 0x00, INPUT_BUFF_SIZE);

		printf("$ ");
		fflush(stdout);

		// nacteni vstupu a kontrola delky
		if ((length = read(0, input, INPUT_BUFF_SIZE)) == INPUT_BUFF_SIZE) {
			printf("Prilis dlouhy vstup!\n");
			while ((ch = getchar()) != '\n' && ch != EOF);	// vyprazdnime stdin		
			continue;
		}

		// odradkovani
		if (strcmp(input, "\n") == 0) {
			continue;
		}

		// konec
		if (strcmp(input, "exit\n") == 0) {
			errFlag = 1;
			Signalize();
			return NULL;
		}

		// zarazka
		input[INPUT_BUFF_SIZE - 1] = '\n';

		// signalizuj, ze vstup je pripraven
		Signalize();

		// Pockej na dokonceni druheho vlakna
		Wait();
	}

	return NULL;
}

// ukonceni procesu na popredi
void sigintHandler(int signum) {

	pthread_mutex_lock(&mutex);
	if (foregroundProcessId > 0) {
		kill(foregroundProcessId, SIGINT);
	}
	pthread_mutex_unlock(&mutex);

}

// tisk informace o potomku
void sigchldHandler(int signum) {
	int retVal;
	pid_t pid = wait(&retVal);

	if (pid <= 0) {
		return;
	}

	// tisk informace 
	if (WIFEXITED(retVal))
		printf("Id: %d skoncil s navratovou hodnotou: %d\n$ ", pid, WEXITSTATUS(retVal));
	else if (WIFSIGNALED(retVal))
		printf("Id: %d ukoncen signalem: %d\n$ ", pid, WTERMSIG(retVal));
	else if (WIFSTOPPED(retVal))
		printf("Id: %d pozastave signalem: %d\n$ ", pid, WSTOPSIG(retVal));
	else if (WIFCONTINUED(retVal)) {
		printf("Id: %d pokraccuje\n$ ", pid);
	}
	fflush(stdout);
}

int main(int argc, char **argv) {
	errFlag = 0;
	pthread_t readThread, cmdThread;
	struct sigaction sigint, sigchld;
	sigset_t set;

	// inicializace signalu
	memset(&sigint, '\0', sizeof(sigint));
	memset(&sigchld, '\0', sizeof(sigchld));

	// vyprazdni set
	sigemptyset(&set);

	// nastav struktury
	sigint.sa_handler = sigintHandler;
	sigint.sa_mask = set;
	sigint.sa_flags = 0;
	sigint.sa_restorer = NULL;

	sigchld.sa_handler = sigchldHandler;
	sigchld.sa_mask = set;
	sigchld.sa_flags = 0;
	sigchld.sa_restorer = NULL;

	// registruj signaly
	sigaction(SIGINT, &sigint, NULL);
	sigaction(SIGCHLD, &sigchld, NULL);

	// inicializce vlaken
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&condition, NULL);

	// vytvoreni vlaken
	pthread_create(&readThread, NULL, readThreadHandler, NULL);
	pthread_create(&cmdThread, NULL, cmdThreadHandler, NULL);

	// cekani na dokonceni vlaken
	pthread_join(readThread, NULL);
	pthread_join(cmdThread, NULL);

	// uvolneni
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&condition);

	return 0;
}

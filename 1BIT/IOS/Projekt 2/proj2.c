/*
 * Soubor:  proj2.c
 * Datum:   2014/05/04
 * Autor:   Lukas Pelanek, xpelan03@stud.fit.vutbr.cz
 * Projekt: Synchronizace procesu, projekt c. 2 pro predmet IOS
 * Popis:   Program resi synchronizacni River Crossing Problem.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <fcntl.h>
#include <time.h>

//Struktura pro ulozeni zadanych parametru 
typedef struct
{
	int count;
	int hacker_wait;
	int serf_wait;
	int sail_time;
	
}parameters;

//Struktura sdilenne pameti 
typedef struct
{
	int action_number;
	int processes1;
	int processes2;
	int boat_members;
	int pier_members;
	int hacker_count;
	int serf_count;
	int landed;
	int landed2;
	int members;
	sem_t *control;
    sem_t *wait_for_boarding;
    sem_t *hacker_boarding;
    sem_t *serf_boarding;
    sem_t *rank;
    sem_t *landing;
    sem_t *write;
    sem_t *sail;
    sem_t *finish;
	
}shared;

//Funkce pro vypis do souboru
void print(const char *message, FILE *out,  ...) 
{
    va_list list;
    va_start (list, out);

    vfprintf(out, message, list);
    
    fflush(NULL);  //vyprazdneni bufferu
    
    va_end(list);
}

void hacker(int id, shared *data, parameters params, FILE *out)
{
	int is_captain =0;  
	int sleep;  			 //doba plavby 
	
	sem_wait(data->control);
		sem_wait(data->write);
			print("%d: hacker: %d: started\n",out, data->action_number++, id);	
		sem_post(data->write);
	sem_post(data->control);
	
	sem_wait(data->wait_for_boarding);
		sem_wait(data->control);
			sem_wait(data->write);
				++data->hacker_count;
				++data->pier_members;	
				print("%d: hacker: %d: waiting for boarding: %d: %d\n",out, data->action_number++, id, data->hacker_count, data->serf_count);

	if(data->pier_members >= 4 && ((data->hacker_count == 4) || (data->serf_count >= 2 && data->hacker_count == 2)))
	{
		is_captain = 1;	                 //posledni kdo vstoupil na molo a utvoril skupinu se stava kapitanem
		if(data->hacker_count == 4)
		{	
			data->hacker_count = data->hacker_count - 4;
			
			for(int a = 0; a < 4; a++)
			{
				sem_post(data->hacker_boarding);     //jestlize skupinu tvori 4 hackeri, pustim je na palubu
			}
				sem_post(data->write);
			sem_post(data->control);
		}    
		else
		{
			data->hacker_count = data->hacker_count - 2;
			data->serf_count = data->serf_count - 2;
			sem_post(data->control);	
			for(int a = 0; a < 2; a++)
			{
				sem_post(data->hacker_boarding);       //jestlize skupinu tvori 2 hackeri a 2 serfove, pustim je na palubu
				sem_post(data->serf_boarding);
			}
				sem_post(data->write);
			sem_post(data->control);
		}	
	}
	else
	{
				sem_post(data->write);
			sem_post(data->control);			
		sem_post(data->wait_for_boarding);   			//jestlize nebyla vytvorena skupina, pustim dalsi osobu
	}
	
	sem_wait(data->hacker_boarding);
		sem_wait(data->control);
			sem_wait(data->write);               //nalodeni
				data->boat_members++;
				print("%d: hacker: %d: boarding: %d: %d\n",out, data->action_number++, id,data->hacker_count, data->serf_count);
		sem_post(data->write);
	sem_post(data->control);
	
	sem_wait(data->control);
	if(data->boat_members == 4)
	{
		data->pier_members = data->pier_members-4;
		data->boat_members = 0;                        //kdyz se nalodi vsichni clenove, otevru semafor
		
		for (int b = 0; b < 4; b++)
		{
			sem_post(data->rank);
		}
		sem_post(data->control);
	}
	else
	{
		sem_post(data->control);
	}
	
	sem_wait(data->rank);
		sem_wait(data->control);
			sem_wait(data->write);
				data->members++;
	if(is_captain == 1)							//rozdeleni hodnosti
	{
	
	print("%d: hacker: %d: captain\n",out, data->action_number++, id);				
	sem_post(data->write);
	}
	else
	{
	print("%d: hacker: %d: member\n",out, data->action_number++, id);
	sem_post(data->write);
	}
	sem_post(data->control);
	
	sem_wait(data->control);
	if(data->members == 4)
	{
		data->members=0;
		
		for(int a =0; a <4; a++)                  //cekam dokud nejsou rozdeleny hodnosti
		{
			sem_post(data->sail);
		}
	  sem_post(data->control);
	}
	else
	{
		sem_post(data->control);
	}
	
	sem_wait(data->sail);
		sem_wait(data->control);
	if(is_captain == 1)
	{
		if (params.sail_time > 0)
		{
		sleep = (rand() % (params.sail_time+1));    //simulovani plavby uspanim procesu kapitana
		usleep(sleep * 1000);
		}
		
		for(int c = 0; c < 4; c++)
		{
			sem_post(data->landing);
		}
		sem_post(data->control);
	}
	else
	{
		sem_post(data->control);
	}
	
	sem_wait(data->landing);
		sem_wait(data->control);
			sem_wait(data->write);
				++data->landed;				//po skonceni plavby se vsechny osoby vylodi
				++data->landed2;
				print("%d: hacker: %d: landing: %d: %d\n",out, data->action_number++, id,data->hacker_count, data->serf_count);
		sem_post(data->write);
	sem_post(data->control);
	
	sem_wait(data->control);
	if (data->landed == 4)
	{
		
		data->landed = 0;
		sem_post(data->control);			
		sem_post(data->wait_for_boarding);			//jestlize vsechny osoby vystoupily z lodi, povolim semafor a dalsi osoby mohou prichazet na molo
	}
	else
	{
	sem_post(data->control);
	}
	
	sem_wait(data->control);
	if(data->landed2 == params.count*2)
	{
		data->landed2 =0;
		for(int a =0; a< params.count*2; a++)	//pokud pocet prepravenych osob*2 je roven poctu generovanych osob od kazde osoby, vsechny osoby byly prepraveny
		{
			sem_post(data->finish);
		}
		sem_post(data->control);
	}
	else
	{
		sem_post(data->control);
	}
	
	sem_wait(data->finish);
		sem_wait(data->control);
			sem_wait(data->write);				
				print("%d: hacker: %d: finished\n",out, data->action_number++, id);			//ukoncuji procesy
		sem_post(data->write);
	sem_post(data->control);		
	
	sem_wait(data->control);	
		data->processes1++;					//inkrementace pocitadla ukoncenych procesu
	sem_post(data->control);
	
	exit(0);
}

void serf(int id, shared *data, parameters params, FILE *out)
{
	int is_captain =0;    //doba plavby
	int sleep;
	sem_wait(data->control);
		sem_wait(data->write);
			print("%d: serf: %d: started\n",out, data->action_number++, id);
		sem_post(data->write);
	sem_post(data->control);
	
	sem_wait(data->wait_for_boarding);
		sem_wait(data->control);
			sem_wait(data->write);						
				++data->serf_count;
				++data->pier_members;
				print("%d: serf: %d: waiting for boarding: %d: %d\n",out, data->action_number++, id, data->hacker_count, data->serf_count);
	
	if(data->pier_members >= 4 && ((data->serf_count == 4) || (data->hacker_count >= 2 && data->serf_count == 2)))
	{
		is_captain = 1;						//posledni kdo vstoupil na molo a utvoril skupinu se stava kapitanem
		if(data->serf_count == 4)
		{
			data->serf_count = data->serf_count - 4;	
			for(int a = 0; a < 4; a++)
			{
				sem_post(data->serf_boarding);		//jestlize skupinu tvori 4 serfove, pustim je na palubu
			}
				sem_post(data->write);
			sem_post(data->control);
		}
		else
		{
			data->hacker_count = data->hacker_count - 2;
			data->serf_count = data->serf_count - 2;
			sem_post(data->control);		
			for(int a = 0; a < 2; a++)
			{
				sem_post(data->serf_boarding);								
				sem_post(data->hacker_boarding);			//jestlize skupinu tvori 2 hackeri a 2 serfove, pustim je na palubu
			}
				sem_post(data->write);
			sem_post(data->control);
		}	
	}
	else
	{
				sem_post(data->write);
			sem_post(data->control);			
	    sem_post(data->wait_for_boarding);					//jestlize nebyla vytvorena skupina, pustim dalsi osobu
	    
	}	

	sem_wait(data->serf_boarding);	
		sem_wait(data->control);					
			sem_wait(data->write);				//nalodeni
				data->boat_members++;
				print("%d: serf: %d: boarding: %d: %d\n",out, data->action_number++, id,data->hacker_count, data->serf_count);
		sem_post(data->write);
	sem_post(data->control);
	
	sem_wait(data->control);
	if(data->boat_members == 4)
	{
		data->pier_members = data->pier_members-4;
		data->boat_members = 0;								//kdyz se nalodi vsichni clenove, otevru semafor
		
		for(int b = 0; b < 4; b++)
		{
			sem_post(data->rank);
		}
		sem_post(data->control);
	}
	else
	{
		sem_post(data->control);
	}
	
	sem_wait(data->rank);
		sem_wait(data->control);				//rozdeleni hodnosti
			sem_wait(data->write);
			data->members++;
	if(is_captain == 1)
	{
		print("%d: serf: %d: captain\n",out, data->action_number++, id);
		sem_post(data->write);
	}
	else
	{
		print("%d: serf: %d: member\n",out, data->action_number++, id);
		sem_post(data->write);
	}
	sem_post(data->control);
	
	sem_wait(data->control);
	if(data->members == 4)
	{
		data->members=0;						//cekam dokud nejsou rozdeleny hodnosti
		
		for(int a =0; a <4; a++)
		{
			sem_post(data->sail);
		}
		sem_post(data->control);
	}
	else
	{
		sem_post(data->control);
	}
	
	sem_wait(data->sail);
		sem_wait(data->control);
	if(is_captain == 1)
	{
		if (params.sail_time > 0)				//simulovani plavby uspanim procesu kapitana
		{
		sleep = (rand() % (params.sail_time+1));
		usleep(sleep * 1000);
		}
		
		for(int c = 0; c < 4; c++)
		{
			sem_post(data->landing);
		}
		sem_post(data->control);
	}
	else
	{
	sem_post(data->control);
	}
	
	sem_wait(data->landing);
		sem_wait(data->control);
			sem_wait(data->write);
				++data->landed;					//po skonceni plavby se vsechny osoby vylodi
				++data->landed2;
				print("%d: serf: %d: landing: %d: %d\n",out, data->action_number++, id,data->hacker_count, data->serf_count);
		sem_post(data->write);
	sem_post(data->control);
	
	sem_wait(data->control);
	if (data->landed == 4)
	{
		data->landed = 0;
		sem_post(data->control);
		sem_post(data->wait_for_boarding);		//jestlize vsechny osoby vystoupily z lodi, povolim semafor a dalsi osoby mohou prichazet na molo
	}
	else
	{
	sem_post(data->control);
	}
	
	sem_wait(data->control);				//pokud pocet prepravenych osob*2 je roven poctu generovanych osob od kazde osoby, vsechny osoby byly prepraveny
	if(data->landed2 == params.count*2)
	{
		data->landed2 =0;				
		for(int a =0; a< params.count*2; a++)
		{
			sem_post(data->finish);
		}
		sem_post(data->control);
	}
	else
	{
		sem_post(data->control);
	}
	
	sem_wait(data->finish);
		sem_wait(data->control);			//ukoncuji procesy
			sem_wait(data->write);
				print("%d: serf: %d: finished\n",out, data->action_number++, id);
		sem_post(data->write);
	sem_post(data->control);
	
	sem_wait(data->control);
		data->processes2++;			//inkrementace pocitadla ukoncenych procesu
	sem_post(data->control);
	exit(0);
}

//funkce pro vytvareni hackeru
void create_hacker (shared *data, parameters params, FILE *out)
{	
	int next_hacker;			
			
		for(int i = 1; i < params.count+1; i++)
			{
				next_hacker = (rand() % (params.hacker_wait+1));    //prirazeni nahodne hodnoty
				if(next_hacker > 0)		//jestlize doba generovani > 0, generuji nove hackery po uplynuti nahodne doby ze zadaneho casu
				{	
					usleep(next_hacker * 1000);
				}
				pid_t hacker_process = fork();
				if (hacker_process == 0)
				{
					hacker(i, data, params, out);
				}
			}
		while(data->processes1 - params.count != 0)
		{	
			wait(NULL);				//kontroluji, zda vsechny procesy jiz byly ukonceny
		}	
	
	exit(0);	
}

//funkce pro vytvareni serfu
void create_serf (shared *data, parameters params, FILE *out)
{	
	int next_serf;
		for(int i = 1; i < params.count+1; i++)
			{
				next_serf = (rand() % (params.serf_wait+1));	//prirazeni nahodne hodnoty	
				if(next_serf > 0)   //jestlize zadany argument je vetsi > 0, generuji nove hackery po uplynuti nahodne doby ze zadaneho casu
				{
			usleep(next_serf * 1000);
				}
			pid_t serf_process = fork();
				if (serf_process == 0)
				{
					serf(i, data, params, out);
				}
			}	
		while(data->processes2 - params.count != 0)
		{	
			wait(NULL);			//kontroluji, zda vsechny procesy jiz byly ukonceny
		}	
	exit(0);	
}

//hlavni proces
void create_main_process(shared *data, parameters params, FILE *out)
{
	pid_t create_hackers = fork();
		if (create_hackers == 0)
		{
			create_hacker(data, params, out);	//proces vytvareni hackeru
			
		}
		else if (create_hackers > 0)
		{
			pid_t create_serfs = fork();
			if (create_serfs== 0)
			{
				create_serf(data, params, out);	//proces vytvareni serfu
			}
			else if (create_serfs > 0)
			{
				wait(NULL);
			}						//cekam na ukonceni procesu vytvareni
			
		  wait(NULL);

		}
}
//kontrola a nacteni parametru do struktury
int get_params (int argc, char *argv[], parameters *params)
{
	if (argc != 5)
	{
		return 1;
	}	
	char *check = NULL;
	short test = 0;
	
	params->count = strtol(argv[1], &check, 10);
	(*check == '\0')? test = test : ++test;
	params->hacker_wait = strtol(argv[2], &check, 10);
	(*check == '\0')? test = test : ++test;
	params->serf_wait = strtol(argv[3], &check, 10);
	(*check == '\0')? test = test : ++test;
	params->sail_time = strtol(argv[4], &check, 10);
	(*check == '\0')? test = test : ++test;
	
	if(test != 0)
	{
		return 1;
	}
	
	if((params->count % 2 != 0) || (params->hacker_wait > 5000) || (params->serf_wait > 5000) || (params->sail_time > 5000))
	{
		return 1;
	}
	
	if((params->count < 1) || (params->hacker_wait < 0) || (params->serf_wait < 0) || (params->sail_time < 0))
	{
		return 1;
	}
	
	return 0;
	
}

//inicializace semaforu a kontrola, zda se sparvne otevrely
int get_semaphores (shared *structura)
{
	if((structura->control = sem_open("/xpelan03-action_sem", O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED)
	{
		fprintf(stderr, "Cannot initialize a semaphore!\n");
		return 1;
	}
    if((structura->wait_for_boarding = sem_open("/xpelan03-wb_sem", O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED)
    {
		fprintf(stderr, "Wrong arguments!\n");
		return 1;
	}
    if((structura->hacker_boarding = sem_open("/xpelan03-hb_sem", O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED)
    {
		fprintf(stderr, "Cannot initialize a semaphore!\n");
		return 1;
	}
    if((structura->serf_boarding = sem_open("/xpelan03-sb_sem", O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED)
    {
		fprintf(stderr, "Cannot initialize a semaphore!\n");
		return 1;
	}
	if((structura->rank = sem_open("/xpelan03-rank_sem", O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED)
	{
		fprintf(stderr, "Cannot initialize a semaphore!\n");
		return 1;
	}
	if((structura->landing = sem_open("/xpelan03-landing_sem", O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED)
	{
		fprintf(stderr, "Cannot initialize a semaphore!\n");
		return 1;
	}
	if((structura->write = sem_open("/xpelan03-write_sem", O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED)
	{
		fprintf(stderr, "Cannot initialize a semaphore!\n");
		return 1;
	}
	if((structura->sail = sem_open("/xpelan03-sail_sem", O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED)
	{
		fprintf(stderr, "Cannot initialize a semaphore!\n");
		return 1;
	}
	if((structura->finish = sem_open("/xpelan03-finish_sem", O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED)
	{
		fprintf(stderr, "Cannot initialize a semaphore!\n");
		return 1;
	}
	structura->action_number = 1; //nastaveni pocatecni hodnoty ciselniku
		
	return 0;
}

//zruseni semaforu 
void unlink_memsem()
{
	sem_unlink("/xpelan03-action_sem");
    sem_unlink("xpelan03-wb_sem");
	sem_unlink("/xpelan03-hb_sem");
	sem_unlink("/xpelan03-sb_sem");
	sem_unlink("/xpelan03-rank_sem");
	sem_unlink("/xpelan03-landing_sem");
	sem_unlink("/xpelan03-write_sem");
	sem_unlink("/xpelan03-sail_sem");
	sem_unlink("/xpelan03-finish_sem");
}


int main(int argc, char *argv[])
{		
	parameters params;
	if (get_params(argc, argv, &params) == 1) 		//kontroluji, zda vstupni parametry jsou validni
		{
			fprintf(stderr, "Wrong arguments!\n");
			return 1;
		}
	
	FILE *out;
	if ((out = fopen("rivercrossing.out", "w+"))==NULL)  //testuji, zda se podarilo otevrit soubor
	{
        fprintf(stderr, "Cannot open file!\n");
        return 2;
	}
	
	int shm_fd;				//Otevreni sdilene pameti, pokud se nepodarilo otevrit, pokusim se uzavrit pamet, pokud zustala otevrena
    if((shm_fd = shm_open("/xpelan03-memory", O_CREAT | O_EXCL | O_RDWR, 0644)) < 0)
    {
		fprintf(stderr, "Cannot open shared memory!\n");
		shm_unlink("/xpelan03-memory");
		unlink_memsem(); 
		fclose(out);
		return 2;
	}	
    
	if(ftruncate(shm_fd, sizeof(shared)) < 0)   
	{
		fprintf(stderr, "Truncate error!\n");
		shm_unlink("/xpelan03-memory");
		close(shm_fd);
		fclose(out);
		return 2;
	}
	shared *structura;
		
	if((structura = mmap(NULL, sizeof(shared), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0)) == MAP_FAILED)
	{
		fprintf(stderr, "Mapping error!\n");
		shm_unlink("/xpelan03-memory");
		close(shm_fd);
		fclose(out);
		return 2;
	}
	
	if(get_semaphores(structura) ==1)
	{
		unlink_memsem();
		return 2;
	}
	
    srand(time(NULL));  //inicializace seedu pro generovani nahodnych cisel
    
    create_main_process(structura, params, out);
    
    fclose(out);
    munmap(structura, sizeof(shared));   //pokud program probehl korektne, vsechno zase uzavru
    shm_unlink("/xpelan03-memory");
	unlink_memsem();   
    close(shm_fd);	
	
    return 0;
}

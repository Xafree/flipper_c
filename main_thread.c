#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

void *routine_flipper(void *arg) {
    pthread_t t_monnayeur;
    pthread_t t_client;
    sem_t semaphore;

	pthread_create(& t_monnayeur, NULL,routine_monnayeur, NULL );


	pthread_exit(EXIT_SUCCESS);
}

void *routine_monnayeur( void * arg){



    pthread_exit(EXIT_SUCCESS);
}


void *routine_client(){


    pthread_exit(EXIT_SUCCESS);
}

int main(void) {
	// Création de la variable qui va contenir le thread
	pthread_t flipper;
	printf("Avant la création du thread.\n");
	// Création du thread
	pthread_create(&flipper, NULL, routine_flipper, NULL);
	printf("Après la création du thread.\n");
	return EXIT_SUCCESS;
}
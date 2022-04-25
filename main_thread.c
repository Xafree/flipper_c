#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

void *routine_monnayeur( void * arg){

    printf("thread crée monnayeur\n");

    pthread_exit(NULL);
}


void *routine_client(){
    printf("thread crée client\n");

    pthread_exit(NULL);
}

void *routine_flipper(void *arg) {
    pthread_t t_monnayeur;
    pthread_t t_client;
    sem_t semaphore;

    printf("nous somme dans le thread \n");

	pthread_create(& t_monnayeur, NULL, &routine_monnayeur, NULL );
    pthread_join(t_monnayeur, NULL);



    pthread_create(& t_client, NULL, &routine_client, NULL );
    pthread_join(t_monnayeur, NULL);

	pthread_exit(NULL);
    free(&t_monnayeur);
}



int main(void) {
	// Création de la variable qui va contenir le thread
	pthread_t flipper;
	printf("Avant la création du thread.\n");

	// Création du thread
	pthread_create(&flipper, NULL, &routine_flipper, NULL);
    pthread_join(flipper, NULL);

	printf("Après la création du thread.\n");

	return EXIT_SUCCESS;
    free(&flipper);
}
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>


sem_t semaphore;
#define MAX_LENGHT 10;


void *routine_partie( void * arg){
    sem_wait(&semaphore);
    printf("thread crée partie\n");
    fflush(stdout);
    sleep(1);
    printf("Score du joueur\n");
    fflush(stdout);
    sem_post(&semaphore);
    pthread_exit(NULL);
}

void *routine_monnayeur( void * arg){
    
    int nb_partie = *(int*) arg;
    pthread_t t_parties[10];
    printf("NB PARTIE %d",nb_partie);
    printf("thread crée monnayeur\n");
    for(int i = 0; i< nb_partie; i++){
        printf("parti %d \n",i);
        pthread_create(&t_parties[i], NULL, &routine_partie, NULL);
        pthread_join(t_parties[i], NULL);
    }

    pthread_exit(NULL);
}


void *routine_client(){
    printf("thread crée client\n");

    pthread_exit(NULL);
}


void *routine_flipper(void *arg) {
    pthread_t t_monnayeur;
    pthread_t t_client;

    int nb_partie = *(int *)arg;
    printf("NB PARTIE %d",nb_partie);

    printf("nous somme dans le thread \n");

    //création du thread monnayeur
	pthread_create(& t_monnayeur, NULL, &routine_monnayeur,&nb_partie);
    pthread_join(t_monnayeur, NULL);

    pthread_create(& t_client, NULL, &routine_client, NULL );
    pthread_join(t_monnayeur, NULL);

	pthread_exit(NULL);
    free(&t_monnayeur);
}



int main(void) {
	// Création de la variable qui va contenir le thread
	pthread_t flipper;
    sem_init(&semaphore,0,1);
    int nb_partie = 5;
	printf("Avant la création du thread.\n");

	// Création du thread
	pthread_create(&flipper, NULL, &routine_flipper, &nb_partie);
    pthread_join(flipper, NULL);

	printf("Après la création du thread.\n");

	return EXIT_SUCCESS;
    free(&flipper);
}
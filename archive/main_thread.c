#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

sem_t semaphore;

struct args_struct {
    sem_t* semaphore;
    int* rand;
};

void *routine_monnayeur( void * arg){

    int* rand = (int *) arg;
    // sem_wait(&semaphore);
    printf("thread crée monnayeur\n");
    printf("random : %d", *rand);
    //sem_post(&semaphore);
    pthread_exit(NULL);
    free(arg);
}


void *routine_client(){
    printf("thread crée client\n");

    pthread_exit(NULL);
}

int get_random (int max){
   double val;
 
   val = (double) max * rand ();
   val = val / (RAND_MAX + 1.0);
 
   return (int) val;
}

void *routine_flipper(void *arg) {
    pthread_t t_monnayeur;
    pthread_t t_client;
    sem_t semaphore;

    //Init du sémaphore
    sem_init(&semaphore,0,1);  
    int rand = get_random(10);
    //initialisation de la struc qui va nous permettre de passé plusieur argument
    //struct args_struct multi_arg = {&semaphore,&rand};
    
    //sem_init(&semaphore,0,1);
    printf("nous somme dans le thread \n");

    //création du thread monnayeur
	pthread_create(& t_monnayeur, NULL, &routine_monnayeur,&rand);
    pthread_join(t_monnayeur, NULL);

    pthread_create(& t_client, NULL, &routine_client, NULL );
    pthread_join(t_monnayeur, NULL);

	pthread_exit(NULL);
    free(&t_monnayeur);
    free(&rand);
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
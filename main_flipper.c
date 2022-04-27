#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>


sem_t semaphore;

typedef struct thread_data
{
    int output;
} thread_data;

int get_random (int max){
   double val;
 
   val = (double) max * rand ();
   val = val / (RAND_MAX + 1.0);
 
   return (int) val;
}


void *routine_partie( void * arg){
    sem_wait(&semaphore);
    printf("thread crée partie\n");
    thread_data *tdata = (thread_data *) arg;
    tdata->output = get_random(1000000);
    sleep(1);
    printf("Score du joueur : %d\n", tdata->output);
    
    sem_post(&semaphore);
    pthread_exit(NULL);
}

void *routine_monnayeur( void * arg){
    
    int nb_partie = *(int*) arg;
    //int *output = malloc(nb_partie * sizeof(int));
    thread_data *tdata;
    //int tab_size = malloc(nb_partie * sizeof(int));
    pthread_t *t_parties = malloc(nb_partie * sizeof(pthread_t));
    printf("thread crée monnayeur\n");
    for(int i = 0; i< nb_partie; i++){
        printf("parti %d \n",i);
        pthread_create(&t_parties[i], NULL, &routine_partie, (void *)&tdata);
        pthread_join(t_parties[i], NULL);
    }
     printf("VALEUR DE OUTPUT : %d \n", &tdata->output);
    free(t_parties);
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
    int nb_partie = 1;

    while(nb_partie != 0){
        printf( "Combien de pièce avez vous-saisie ? \n" );
        scanf( "%d", &nb_partie );

        if( nb_partie >= 1000 || nb_partie == 0){
            if(nb_partie == 0){
                printf("Aurevoir !\n");
            }else{
                printf("Le nombre de partie maximal est de 10 !\n");
            }
            
        }else{
            printf("Avant la création du thread.\n");

            // Création du thread
            pthread_create(&flipper, NULL, &routine_flipper, &nb_partie);
            pthread_join(flipper, NULL);

            printf("Après la création du thread.\n");
        }   
        
    }
    
    return EXIT_SUCCESS;
    free(&flipper);
}
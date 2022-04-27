#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>


sem_t semaphore;

typedef struct thread_data
{
    int output;
    int nb_partie;
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
    
    int nb_partie;
    thread_data *tdata = (thread_data *) arg;
    printf( "Combien de pièce avez vous-saisie ? \n" );
    fflush(stdin);
    scanf( "%d", nb_partie );
    printf("TEST");
    tdata->nb_partie = &nb_partie;
    
    pthread_exit(NULL);
}


void *routine_client(){
    printf("thread crée client\n");

    pthread_exit(NULL);
}


void *routine_flipper(void *arg) {
    pthread_t t_monnayeur;
    pthread_t t_client;

    //int nb_partie = *(int *)arg;

    printf("nous somme dans le thread \n");
    thread_data *tdata;
    //création du thread monnayeur
	pthread_create(& t_monnayeur, NULL, &routine_monnayeur,(void *)&tdata);
    pthread_join(t_monnayeur, NULL);

    //lunch game
    //int nb_partie = *(int*) arg;
    //int *output = malloc(nb_partie * sizeof(int));
    
    //int tab_size = malloc(nb_partie * sizeof(int));
    printf("test ici retour lenght %d \n",tdata->nb_partie);
    int max_lengt = tdata->nb_partie;
    pthread_t *t_parties = malloc(max_lengt * sizeof(pthread_t));
    for(int i = 0; i< tdata->nb_partie ; i++){
        printf("parti %d \n",i);
        pthread_create(&t_parties[i], NULL, &routine_partie, (void *)&tdata);
        pthread_join(t_parties[i], NULL);
    }

    printf("VALEUR DE OUTPUT : %d \n", &tdata->output);

    pthread_create(& t_client, NULL, &routine_client, NULL );
    pthread_join(t_monnayeur, NULL);

	pthread_exit(NULL);
    free(t_parties);
    free(&t_monnayeur);
}



int main(void) {
	// Création de la variable qui va contenir le thread
	pthread_t flipper;
    sem_init(&semaphore,0,1);
    int nb_partie = 1;

    while(nb_partie != 0){


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
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>


sem_t semaphore;

struct thread_datas {
  int nb_piece;
  int socre;
};

int get_random (int max){
   double val;
 
   val = (double) max * rand ();
   val = val / (RAND_MAX + 1.0);
 
   return (int) val;
}


void *routine_partie( void * arg){
    sem_wait(&semaphore);
    struct thread_datas *args = (struct thread_datas *) arg;

    int score = get_random(1000000);
    args->socre =score;
    sleep(1);

    sem_post(&semaphore);
    pthread_exit((void *)(args->socre));
}

void *routine_monnayeur( void * arg){

    printf("dans le thread monnayeur \n");
    struct thread_datas *args = (struct thread_datas *) arg;
    int nb_piece;
    printf("Combien de pièces avez vous mis ? \n");
    scanf("%d", &nb_piece);
    args->nb_piece = nb_piece;
    pthread_exit((void *)(args->nb_piece));
}


void *routine_client(void * arg){
    printf("Dans du thread client \n");
    int *args = (int *) arg;
    int length  = sizeof(args)/sizeof(args[0]);

    for(int i = 0;i<length;i++){
        printf("tab_socre[%d] : %d \n",i,args[i]);
    }
    pthread_exit(NULL);
}


void *routine_flipper(void *arg) {
    //Variable
    pthread_t t_monnayeur;
    pthread_t t_client;
    struct thread_datas *args = calloc (sizeof (struct thread_datas), 1);

    //Thread monnayeur
    void *nb_piece = NULL;
    printf("Création du thread monnayeur \n");
	pthread_create(& t_monnayeur, NULL, &routine_monnayeur,args);
    pthread_join(t_monnayeur, &nb_piece);

    //TODO playeur
    pthread_t *t_parties = malloc((int) nb_piece* sizeof(pthread_t));
    int *tab_score = malloc((int) nb_piece* sizeof(int));
    void *score = NULL;
    for(int i = 0; i< (int) nb_piece ; i++){
        pthread_create(&t_parties[i], NULL, &routine_partie,args);
        pthread_join(t_parties[i], &score);
        tab_score[i] = (int) score;
        printf ("score de la partie n°%d thread_flipper : %d\n",i,(int) score);
    }
    
    //Thread client
    printf("Création du thread client \n");
    pthread_create(& t_client, NULL, &routine_client,tab_score);
    pthread_join(t_monnayeur, NULL);

    //Kill du thread
    free(args);
    pthread_exit(NULL);
}


int main(void) {
	// Création de la variable qui va contenir le thread
	pthread_t flipper;
    sem_init(&semaphore,0,1);

    printf("Création du thread flipper \n");

    pthread_create(&flipper, NULL, &routine_flipper, NULL);
    pthread_join(flipper, NULL);

    printf("Fin du thread flipper \n");
}
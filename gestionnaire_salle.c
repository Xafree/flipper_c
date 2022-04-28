#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t semaphore;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

struct thread_datas {
  int tab_score[5]; // mettre un pointeur quand vrai data 
};


void *routine_afficheur(void * arg){
    struct thread_datas *args = (struct thread_datas *) arg;
    //int tab_score[5] = args->tab_score;

    for(int i = 0; i<5 ; i++){
        printf("score %d : %d \n",i, args->tab_score[i]);
    }

}

void *routine_serveurTCP(void * arg){
    struct thread_datas *args = (struct thread_datas *) arg;
    printf("Waiting response.....\n");
    sleep(2);
    //simulation de valeur
    for(int i = 0; i < 5; ++i) {
        args->tab_score[i]=i+3*4;
    }
    
    printf("Success\n");
    pthread_exit((void *)(args->tab_score));
}

void *routine_gestionnaire_salle(void *arg) {
    pthread_t t_serveur_tcp;
    pthread_t t_afficheur;
    struct thread_datas *args = calloc (sizeof (struct thread_datas), 1);

    printf("Création du thread serveur TCP \n");
    void* tableau = NULL;
    pthread_create(&t_serveur_tcp, NULL, &routine_serveurTCP, args);
    pthread_join(t_serveur_tcp, &tableau);

    printf("Création du thread Afficheur \n");
    pthread_create(&t_afficheur, NULL, &routine_afficheur, args);
    pthread_join(t_afficheur, NULL);

    pthread_exit(NULL);
}

int main(void) {
	// Création de la variable qui va contenir le thread
	pthread_t t_gestionnaire_salle;
    sem_init(&semaphore,0,1);

    printf("Création du thread gestionnaire de salle \n");

    pthread_create(&t_gestionnaire_salle, NULL, &routine_gestionnaire_salle, NULL);
    pthread_join(t_gestionnaire_salle, NULL);

    printf("Fin du thread gestionnaire de salle  \n");
}
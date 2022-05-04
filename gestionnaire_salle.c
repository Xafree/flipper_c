#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

sem_t semaphore;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_t writerthreads[100];
pthread_t readerthreads[100];

struct thread_datas {
  int tab_score[3]; // mettre un pointeur quand vrai data 
  int index;
};


void *routine_afficheur(void * arg){
    struct thread_datas *args = (struct thread_datas *) arg;
    //int tab_score[5] = args->tab_score;

    while(1) {
        sleep(3);
        sem_wait(&semaphore);
        printf("\n");
        for(int i = 0; i < 3 ; i++){
            printf("SCORE AFFICHEUR %d : %d \n",i, args->tab_score[i]);
        }
        sem_post(&semaphore);
    }
}


void *routine_serveurTCP(void * arg){

    struct thread_datas *args = (struct thread_datas *) arg;
// Initialize variables
    int serverSocket, newSocket;
    struct sockaddr_in serverAddr;
    struct sockaddr_storage serverStorage;
 
    socklen_t addr_size;
    
 
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8989);
 
    // Bind the socket to the
    // address and port number.
    bind(serverSocket,
         (struct sockaddr*)&serverAddr,
         sizeof(serverAddr));
 
    // Listen on the socket,
    // with 40 max connection
    // requests queued
    if (listen(serverSocket, 50) == 0)
        printf("Listening\n");
    else
        printf("Error\n");
 
    // Array for thread
    //pthread_t tid[60];
 
    int i = 0;
 
    while (1) {
        addr_size = sizeof(serverStorage);
 
        // Extract the first
        // connection in the queue
        newSocket = accept(serverSocket,
                           (struct sockaddr*)&serverStorage,
                           &addr_size);
        int score = 0;
        recv(newSocket,
             &score, sizeof(score), 0);

        // // Lock the semaphore
        sem_wait(&semaphore);
        printf("\nThread ServerTCP : Adding value to buffer \n");
        // Unlock the semaphore
        //printf("\nThe score is: %d\n", score);

        args->tab_score[args->index] = score;

        if (args->index < 2) {
            args->index++;
        } else {
            args->index = 0;
        }

        // for (int i = 0 ; i < 3 ; i++) {
        //     printf("score %d : %d \n", i, args->tab_score[i]);
        // }

        sem_post(&semaphore);

    }
    printf("Success\n");
    pthread_exit((void *)(args->tab_score));
}

void *routine_gestionnaire_salle(void *arg) {
    pthread_t t_serveur_tcp;
    pthread_t t_afficheur;
    sem_init(&semaphore, 0, 1);
    struct thread_datas *args = calloc (sizeof (struct thread_datas), 1);
    
    args->index = 0;

    for (int i= 0 ; i < 3 ; i++) {
        args->tab_score[i] = 0;
    }

    // printf("THIS IS INIT TABLEAU\n");

    // for (int i = 0 ; i < 3 ; i++) {
    //     printf("score %d : %d \n", i, args->tab_score[i]);
    // }

    printf("Création du thread serveur TCP \n");
    pthread_create(&t_serveur_tcp, NULL, &routine_serveurTCP, args);

    printf("Création du thread Afficheur \n");
    pthread_create(&t_afficheur, NULL, &routine_afficheur, args);
    
    
    pthread_join(t_serveur_tcp, NULL);
    pthread_join(t_afficheur, NULL);


    pthread_exit(NULL);
}


int main(void) {
	// Création de la variable qui va contenir le thread

	pthread_t t_gestionnaire_salle;

    printf("Création du thread gestionnaire de salle \n");

    pthread_create(&t_gestionnaire_salle, NULL, &routine_gestionnaire_salle, NULL);
    pthread_join(t_gestionnaire_salle, NULL);

    printf("Fin du thread gestionnaire de salle  \n");
}
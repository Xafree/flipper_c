#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#include <arpa/inet.h>

sem_t semaphore;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

struct thread_datas {
  int tab_score[3]; // mettre un pointeur quand vrai data 
  int newSocket;
  int score;
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
    int score = args->score;
    recv(args->newSocket, &score, sizeof(args->score), 0);
    printf("Waiting response.....\n");
    
    printf("Score : %d", args->score);
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
    //Socket
    int serverSocket, newSocket;
    struct sockaddr_in serverAddr;
    struct sockaddr_storage serverStorage;
    socklen_t addr_size;

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8989);

    bind(serverSocket,(struct sockaddr*)&serverAddr, sizeof(serverAddr));
 
    if (listen(serverSocket, 50) == 0)
        printf("Listening\n");
    else
        printf("Error\n");
    // Array for thread
    pthread_t tid[60];
    pthread_t t_serveur_tcp[100];
    pthread_t t_afficheur[100];
    struct thread_datas *args = calloc (sizeof (struct thread_datas), 1);


    while (1) {
        addr_size = sizeof(serverStorage);
        // Extract the first
        // connection in the queue
        newSocket = accept(serverSocket, (struct sockaddr*)&serverStorage, &addr_size);
        args->newSocket = newSocket;
        args->score = 0;
        //recv(newSocket, &score, sizeof(score), 0);
        pthread_create(&t_serveur_tcp, NULL, routine_serveurTCP, args);
    }
}
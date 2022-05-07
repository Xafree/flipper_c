#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

sem_t semaphore;

struct thread_datas {
  int buffer_score[3];
  int index;
  int record;
  int updatedBuffer;
};


void *routine_afficheur(void * arg){
    struct thread_datas *args = (struct thread_datas *) arg;
    const int size = 5;
    int tab_score_afficheur[5] = {0, 0, 0, 0, 0};
    int index_afficheur = 0;

    while(1) {
        
        if (args->updatedBuffer == 1) {
            sem_wait(&semaphore);
            printf("\n");

            if (index_afficheur >= size) {
                index_afficheur = 0;
            }

            if (args->index >= 0) {

                tab_score_afficheur[index_afficheur] = args->buffer_score[args->index];
                index_afficheur++;
            }

                
            for (int j = 0 ; j < size ; j++) {
                printf("SCORE AFFICHEUR %d : %d \n", j, tab_score_afficheur[j]);
            }

            if (args->record >= 0) {
                printf("RECORD: %d\n", args->record);
            } else {
                printf("AUCUN RECORD\n");
            }
            args->updatedBuffer = 0;
            sem_post(&semaphore);
        }
    }

}


void *routine_serveurTCP(void * arg){

    struct thread_datas *args = (struct thread_datas *) arg;
    int serverSocket, newSocket;
    struct sockaddr_in serverAddr;
    struct sockaddr_storage serverStorage;
 
    socklen_t addr_size;
    
 
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8989);
 
    bind(serverSocket,
         (struct sockaddr*)&serverAddr,
         sizeof(serverAddr));
 

    if (listen(serverSocket, 50) == 0)
        printf("Listening\n");
    else
        printf("Error\n");
 
 
    while (1) {
        addr_size = sizeof(serverStorage);
        newSocket = accept(serverSocket,
                           (struct sockaddr*)&serverStorage,
                           &addr_size);
        int score = 0;
        recv(newSocket, &score, sizeof(score), 0);


        sem_wait(&semaphore);
        printf("\nThread ServerTCP : Adding value to buffer \n");


        if (score > args->record)
            args->record = score;

        if (args->index < 2) {
            args->index++;
        } else {
            args->index = 0;
        }

        if (args->index >= 0) 
            args->buffer_score[args->index] = score;

        args->updatedBuffer = 1;
        sem_post(&semaphore);

    }
    printf("Success\n");
    pthread_exit((void *)(args->buffer_score));
}


int main(void) {
	
    pthread_t t_serveur_tcp;
    pthread_t t_afficheur;
    sem_init(&semaphore, 0, 1);
    struct thread_datas *args = calloc (sizeof (struct thread_datas), 1);
    
    args->index = -1;
    args->record = -1;
    args->updatedBuffer = 0;

    for (int i= 0 ; i < 3 ; i++) {
        args->buffer_score[i] = 0;
    }

    printf("Création du thread serveur TCP \n");
    pthread_create(&t_serveur_tcp, NULL, &routine_serveurTCP, args);

    printf("Création du thread Afficheur \n");
    pthread_create(&t_afficheur, NULL, &routine_afficheur, args);
    
    
    pthread_join(t_serveur_tcp, NULL);
    pthread_join(t_afficheur, NULL);

}
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <time.h>

sem_t semaphore;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

struct thread_datas {
  int nb_piece;
  int tmp_score;
  int *tab_score;
};

int get_random (int max){
   double val;
   srand(time(NULL));
   val = (double) max * rand ();
   val = val / (RAND_MAX + 1.0);
 
   return (int) val;
}

void *routine_partie( void * arg){
    sem_wait(&semaphore);
    struct thread_datas *args = (struct thread_datas *) arg;

    int score = get_random(1000000);
    printf("Score partie : %d\n",score);
    args->tmp_score =score;
    sleep(1);

    sem_post(&semaphore);
    pthread_exit((void *)(args->tmp_score));
}

void *routine_monnayeur( void * arg){
    printf("dans le thread monnayeur \n");

    //initialize variable
    struct thread_datas *args = (struct thread_datas *) arg;
    int nb_piece;
    int chance = get_random(100);

    //jobs
    printf("Combien de pièces avez vous mis ? \n");
    scanf("%d", &nb_piece);

    if(chance < 11){
        printf("Bravo vous avez gagnez une partie bonus !\n");
        nb_piece++;
    }

    args->nb_piece = nb_piece;

    //kill Thread
    pthread_exit((void *)(args->nb_piece));
}

void *routine_client(void * arg){
    printf("Dans du thread client \n");
    pthread_mutex_lock(&mutex);
    struct thread_datas *args = (struct thread_datas *) arg;
    int network_socket;

    network_socket = socket(AF_INET,SOCK_STREAM, 0);

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(8989);

    int connection_status = connect(network_socket,
                                    (struct sockaddr*)&server_address,
                                    sizeof(server_address));

    if (connection_status < 0) {
        puts("Error\n");
        return 0;   
    }

    printf("Connection estabilished\n");
 
    // Send data to the socket
    send(network_socket, args->tab_score, sizeof(args->tab_score), 0);

    //printf("Suspense .....\n");
    //sleep(2);
 
    close(network_socket);
    pthread_mutex_unlock(&mutex);
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

    int *tab_score = malloc(sizeof(int));

    void *score = NULL;
    for(int i = 0; i< (int) nb_piece ; i++){

        pthread_mutex_lock(&mutex);

        pthread_create(&t_parties[i], NULL, &routine_partie,args);
        pthread_join(t_parties[i], &score);
        tab_score[0] = (int) score;
        printf ("score de la partie n°%d en cours...\n",i+1);
        
        args->tab_score = tab_score;
        pthread_mutex_unlock(&mutex);

        //Thread client
        printf("Création du thread client \n");
        pthread_create(&t_client, NULL, &routine_client,args);
        pthread_join(t_client, NULL);
    }

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
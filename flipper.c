#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <time.h>

// Initialisation semaphores et variables communes
sem_t sem_credits;
sem_t sem_buffer_score;
int buffer_score[1];
int buffer_updated;
int partie_terminee;

int get_random (int max){
   double val;
   val = (double) max * rand ();
   val = val / (RAND_MAX + 1.0);
 
   return (int) val;
}

void* routine_monayeur(void* arg) {
    printf("\nDEBUT DU THREAD MONAYEUR...\n");

    while(1) {
        
        int sem_val = 0;
        sem_getvalue(&sem_credits, &sem_val);

        // Si les credits du joueurs sont epuises
        if (sem_val <= 0 ) {

            int pieces_inserees = 0;
            printf("\n====================\n");
            printf("\nInserer pieces:\n\n");
            scanf("%d", &pieces_inserees);

            for(int i = 0 ; i < pieces_inserees ; i++) {
                sem_post(&sem_credits);
            }
            partie_terminee = 0;

        }

    }
}

void *routine_clientTCP(void * arg){
    printf("\nDEBUT DU THREAD CLIENT TCP...\n");
 
    while(1) {
        

        if (buffer_updated == 1) {
            sem_wait(&sem_buffer_score);
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

            // Send data to the socket
            printf("\nBUFFER SCORE IN CLIENT TCP : %d\n", buffer_score[0]);
            send(network_socket, buffer_score, sizeof(buffer_score), 0);
            close(network_socket);
            buffer_updated = 0;
            sem_post(&sem_buffer_score);
        }
        
    }

}

int main(void) {
    
    printf("DEBUT DU MAIN THREAD...\n");

    pthread_t t_monnayeur;
    pthread_t t_clientTCP;

    sem_init(&sem_credits, 0, 0);
    sem_init(&sem_buffer_score, 0, 1);

    srand(time(NULL));

    buffer_score[0] = 0;
    buffer_updated = 0;

    pthread_create(&t_clientTCP, NULL, &routine_clientTCP, NULL);
    pthread_create(&t_monnayeur, NULL, &routine_monayeur, NULL);

    while(1) {
        int chance = get_random(100);
        sem_wait(&sem_credits);
        printf("\nPartie en cours...\n");

        sleep(1);
        sem_wait(&sem_buffer_score);
        buffer_score[0] = get_random(1000000);
        buffer_updated = 1;
        sem_post(&sem_buffer_score);

        if(chance < 11){
            printf("\nBravo! Vous avez gagne une partie gratuite!\n");
            sem_post(&sem_credits);
        }

    }

    pthread_join(t_monnayeur, NULL);
    pthread_join(t_clientTCP, NULL);
}   
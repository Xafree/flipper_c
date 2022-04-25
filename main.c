#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define psleep(sec) sleep ((sec))
#define INITIAL_STOCK   20
#define NB_CLIENTS      5
#define INITIAL_SCORE   0


/* Structure stockant les informations des threads clients et du magasin. */
typedef struct
{
   int score_joueur;
   sem_t semaphore;
 
   pthread_t thread_monnayeur;
   pthread_t thread_clients;
}
flipper_t;
 
static flipper_t flipper =
{
   .score_joueur = INITIAL_SCORE,
   
};
 
/* Score random du flipper */
static int get_random (int max)
{
   double val;
 
   val = (double) max * rand ();
   val = val / (RAND_MAX + 1.0);
 
   return ((int) val);
}

static void * fn_clients ()
{
    while (1) {
        int val = get_random (6);
        //psleep (get_random (3));
        flipper.score_joueur = val;
        printf ( "Score du joueur : %d \n",flipper.score_joueur );
    }
    
    return NULL;
}

static void * monnayeur (int val, int credit){
    
    // initialisation du semaphore 
    //sem_init(&flipper.semaphore, 0, 1);
    int monnayeurCredit = credit;

    if(val = 1 && !monnayeurCredit){
        monnayeurCredit -= 1;
    }

    

}
 
void main(){

    int ret = 0;

    // Thread qui affiche le score du jouer thread_clients TCP
    ret = pthread_create (& flipper.thread_clients, NULL, monnayeur(1,10), NULL);

    fprintf (stderr, "%d \n", strerror (ret));

}


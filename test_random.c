#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>


int main(){
    double val;
 
   val = (double) 1000000 * rand ();
   val = val / (RAND_MAX + 1.0);
 
   printf("test rand %d : \n", (int)val);
}
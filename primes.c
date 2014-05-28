#include "primes.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <semaphore.h>
#include "circularQueue.h"

#define QUEUE_CAPACITY 5

int * primes; //Array of ints used to store all the primes numbers 
int currentIndex = 0; //Int variable used to keep track of the current index on the primes array
int N; //Argument passed on the program made globally
sem_t stop; //Semaphore used to signal whether or not the prime counting has stoped

int main(int argc, char * argv[], char * envp[]) {

    if (argc < 2) {
        fprintf(stderr, "Usage: %s primeNumber\n", argv[0]);
        exit(1);
    }

    sem_init(&stop, 0, 0); //Initialize semaphore

    N = atoi(argv[1]); //Command line argument that specifies the max number to which the program checks primes
    int numberOfPrimes = ceil(1.2 * ((double) N / log((double) N))); //1.2*(N / ln N) Formula to stablish the number of primes present from 2 to N

    primes = (int *) malloc(sizeof (int) * numberOfPrimes); //Memory allocation for the primes array 

    pthread_t tid; //Initial Thread id
    pthread_create(&tid, NULL, initialThread, NULL); //Thread creation                                           

    sem_wait(&stop);

    if(!currentIndex){ 
    	printf("Argument must be atleast 2.\n");
    	return 0;
    }

    qsort(primes, (size_t)numberOfPrimes, sizeof(int), compare_ints);

    printf("Prime numbers: ");
    fflush(stdout);

    /* Prints prime number after them being calculated */
    int first = 1;
    int i;
    for (i = 0; i < numberOfPrimes; i++) {
        if (primes[i]) {
            if (first) {
            	first = 0;
                printf("%d", primes[i]);
            } else
                printf(", %d", primes[i]);
            fflush(stdout);
        }
    }
    printf("\n");

    sem_destroy(&stop);
    return 0;
}

/* Function launched by the initial thread  */
void *initialThread(void *arg) { 

    if (N > 2) {

    	primes[currentIndex++] = 2; //Adds the initial number to primes

        //Create Exit Queue
        CircularQueue *exitQueue; //Creates circularQueue instance
        queue_init(&exitQueue, QUEUE_CAPACITY); //Initializes exitQueue
        
        //Create Filter Thread
        pthread_t tid; //Filter thread id 
        pthread_create(&tid, NULL, filterThread, exitQueue); //Creates filter thread with fucntion filterThread and the exitQueue

        /* Place odd numbers & 0 */
        int i;
        for (i = 3; i < N; i += 2) {
            queue_put(exitQueue, i);
        }
        if (N % 2) queue_put(exitQueue, N);
        queue_put(exitQueue, 0);

    } else {
        sem_post(&stop); //Signals semaphore to indicate  
    }

    return (void *)NULL;
}

/* Function launched by the filter thread  */
void *filterThread(void *arg) {

    CircularQueue *entryQueue = (CircularQueue *) arg; //Creates circularQueue instance
    int first = queue_get(entryQueue);      


    if (first > (int) sqrt(N)) {
		primes[currentIndex++] = first; //Places first element of the entryQueue to the primes array

        int head;
        while ((head = queue_get(entryQueue)) != 0) {
            primes[currentIndex++] = head;
        }
        sem_post(&stop);
    } else {
        //Create Exit Queue
        CircularQueue *exitQueue; //Creates circularQueue instance
        queue_init(&exitQueue, QUEUE_CAPACITY); //Initializes exitQueue

        //Create Filter Thread
        pthread_t tid; //Filter thread id 
        pthread_create(&tid, NULL, filterThread, exitQueue); //Creates filter thread with fucntion filterThread and the exitQueue

        /* Filter multiples & place non-multiple in the exit queue */
        int head;
        while ((head = queue_get(entryQueue)) != 0) 
            if (head % first)       //if non-multiple of the 1st value
                queue_put(exitQueue, head);
                    
        queue_put(exitQueue, 0);        //Places end caracter on th exitQueue

        primes[currentIndex++] = first; //Places first element of the entryQueue to the primes array
    }

    return (void *)NULL;
}

int compare_ints(const void* a, const void* b)   // comparison function
{
    int arg1 = *(const int*)(a);
    int arg2 = *(const int*)(b);
    if(arg1 < arg2) return -1;
    if(arg1 > arg2) return 1;
    return 0;
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <semaphore.h>
#include "circularQueue.h"

#define QUEUE_CAPACITY 50

void *initialThread(void *arg);
void *filterThread(void *arg);

int * primes;
int currentIndex = 0;
int N; //Argument passed on the program
sem_t stop;
pthread_mutex_t lock;

int main(int argc, char * argv[], char * envp[]) {

    if (argc < 2) {
        fprintf(stderr, "Usage: %s primeNumber\n", argv[0]);
        exit(1);
    }

    //Initialize semaphore
    sem_init(&stop, 0, 0);

    N = atoi(argv[1]);
    int numberOfPrimes = ceil(1.2 * ((double) N / log((double) N))); //1.2*(N / ln N)

    primes = (int *) malloc(sizeof (int) * numberOfPrimes);

    pthread_t tid;
    pthread_create(&tid, NULL, initialThread, NULL);
    pthread_join(tid, NULL);

    sem_wait(&stop);

    printf("Prime numbers: ");
    fflush(stdout);

    int i;
    for (i = 0; i < numberOfPrimes; i++) {
        if (primes[i]) {
            if (i == 0)
                printf("%d", primes[i]);
            else
                printf(", %d", primes[i]);
            fflush(stdout);
        }
    }
    printf("\n");

    sem_destroy(&stop);
    return 0;
}

void *initialThread(void *arg) {

    primes[currentIndex++] = 2;

    if (N > 2) {

        //Create Exit Queue
        CircularQueue *exitQueue;
        queue_init(&exitQueue, QUEUE_CAPACITY);

        //Create Filter Thread
        pthread_t tid;
        pthread_create(&tid, NULL, filterThread, exitQueue);

        //Place odd numbers & 0
        int i;
        for (i = 3; i < N; i += 2) {
            queue_put(exitQueue, i);
        }
        if (N % 2) queue_put(exitQueue, N);
        queue_put(exitQueue, 0);

        //queue_print(exitQueue);

    } else {
        sem_post(&stop);
    }
}

void *filterThread(void *arg) {
    
    pthread_mutex_lock(&lock); //Let each thread finish before proceeding to the next thread

    CircularQueue *entryQueue = (CircularQueue *) arg;
    int first = queue_get(entryQueue);
    primes[currentIndex++] = first;

    if (first > (int) sqrt(N)) {
        int head;
        while ((head = queue_get(entryQueue)) != 0) {
            primes[currentIndex++] = head;
        }
        sem_post(&stop);
    } else {
        //Create Exit Queue
        CircularQueue *exitQueue;
        queue_init(&exitQueue, QUEUE_CAPACITY);

        //Create Filter Thread
        pthread_t tid;
        pthread_create(&tid, NULL, filterThread, exitQueue);

        //Filter multiples & place non-multiple in the exit queue
        int head;
        while ((head = queue_get(entryQueue)) != 0) {
            if (head % first) { //if non-multiple of the 1st value
                queue_put(exitQueue, head);
            }
        }
        queue_put(exitQueue, 0);

        //The Placing of the 1st value in the public list is already done  before the if statement
    }

    pthread_mutex_unlock(&lock);

}


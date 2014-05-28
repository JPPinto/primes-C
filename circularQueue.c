#include "circularQueue.h"

//------------------------------------------------------------------------------------------
// Allocates space for circular queue 'q' having 'capacity' number of elements
// Initializes semaphores & mutex needed to implement the producer-consumer paradigm
// Initializes indexes of the head and tail of the queue
// TO DO BY STUDENTS: ADD ERROR TESTS TO THE CALLS & RETURN a value INDICATING (UN)SUCESS

void queue_init(CircularQueue **q, unsigned int capacity) // TO DO: change return value
{
    *q = (CircularQueue *) malloc(sizeof (CircularQueue));
    sem_init(&((*q)->empty), 0, capacity);
    sem_init(&((*q)->full), 0, 0);
    pthread_mutex_init(&((*q)->mutex), NULL);
    (*q)->v = (QueueElem *) malloc(capacity * sizeof (QueueElem));
    (*q)->capacity = capacity;
    (*q)->first = 0;
    (*q)->last = 0;
}

//------------------------------------------------------------------------------------------
// Inserts 'value' at the tail of queue 'q'

void queue_put(CircularQueue *q, QueueElem value) {
    sem_wait(&q->empty);

    q->v[q->last] = value;
    q->last++;
    q->last %= q->capacity;

    sem_post(&q->full);

}

//------------------------------------------------------------------------------------------
// Removes element at the head of queue 'q' and returns its 'value'

QueueElem queue_get(CircularQueue *q) {
    sem_wait(&q->full);

    QueueElem queueValue = q->v[q->first];
    q->first++;
    q->first %= q->capacity;

    sem_post(&q->empty);

    return queueValue;
}

//------------------------------------------------------------------------------------------
// Frees space allocated for the queue elements and auxiliary management data
// Must be called when the queue is no more needed

void queue_destroy(CircularQueue *q) {
    sem_destroy(&q->empty);
    sem_destroy(&q->full);
    free(q->v);
    free(q);
}

//------------------------------------------------------------------------------------------
// Prints the Queue

void queue_print(CircularQueue *q) {
    printf("Queue:");
    int i;
    for (i = 0; i < q->capacity; i++) {
        printf(" %d", (int) q->v[i]);
        fflush(stdout);
        if (i != q->capacity - 1) printf(",");
    }
    printf("\n");
}

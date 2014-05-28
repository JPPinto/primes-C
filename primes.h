#ifndef __PRIMES_H
#define __PRIMES_H

void *initialThread(void *arg); //Prototype for the fucntion call by the initial thread
void *filterThread(void *arg); //Prototype for the fucntion call by all the filter threads
int compare_ints(const void* a, const void* b);   // Sort function

#endif /* __PRIMES_H */

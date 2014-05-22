#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "circularQueue.h"

int * primes;

int main(int argc, char * argv[], char * envp[]){

	if(argc < 2){
		fprintf(stderr, "Usage: %s primeNumber\n", argv[0]);
        exit(1);
	}

	int amountOfNumbers = atoi(argv[1]);

	int numberOfPrimes = 1.2 * ((double)amountOfNumbers / log((double)amountOfNumbers)); //1.2*(N / ln N)

	primes = (int *)malloc(sizeof(int) * numberOfPrimes); 

	exit(EXIT_SUCCESS);
}

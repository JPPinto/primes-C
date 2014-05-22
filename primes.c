#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cmath>
#include "circularQueue.h"

int * primes;
int currentIndex = 0;

int main(int argc, char * argv[]){

	if(argc < 2){
		fprintf(stderr, "Usage: %s primeNumber\n", argv[0]);
        exit(1);
	}

	int amountOfNumbers = atoi(argv[1]);

	int numberOfPrimes = 1.2 * (amountOfNumbers / log(amountOfNumbers));

	primes = (int *)malloc(sizeof(int) * numberOfPrimes); //1.2*(N / ln N)

	

}

primes: primes.o circularQueue.c circularQueue.h
	cc primes.c circularQueue.c -lm -lpthread -Wall -o primes

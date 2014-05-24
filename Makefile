CFLAGS=-Aa -D_HPUX_SOURCE
primes: primes.c circularqueue.c circularqueue.h
	cc -lm -lpthread -o primesExe -Wall primes.c circularqueue.c 
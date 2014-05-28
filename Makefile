primes: primes.o circularQueue.o
	gcc -o bin/primes bin/primes.o bin/circularQueue.o -lm -lpthread -Wall

primes.o: primes.c
	gcc -c -o bin/primes.o primes.c -lm -lpthread -Wall

circularQueue.o: circularQueue.c
	gcc -c -o bin/circularQueue.o circularQueue.c -lm -lpthread -Wall

clean:
	rm bin/*.o
	rm bin/primes

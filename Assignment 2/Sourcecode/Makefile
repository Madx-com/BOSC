all: sqrtsum

sqrtsum: sqrtsum.o
	gcc -pthread -o sqrtsum sqrtsum.c -lm

sqrtsum.o: sqrtsum.c
	gcc -c sqrtsum.c


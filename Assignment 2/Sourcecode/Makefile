all: sqrtsum

sqrtsum: sqrtsum.o
	gcc -pthread -o sqrtsum sqrtsum.o -lm

sqrtsum.o: sqrtsum.c
	gcc -c sqrtsum.c


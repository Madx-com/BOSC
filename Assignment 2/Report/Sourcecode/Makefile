all: sqrtsum
	cd list; make
	cd banker; make

LIBS = -pthread

sqrtsum: sqrtsum.o
	gcc -o $@ ${LIBS} sqrtsum.o -lm



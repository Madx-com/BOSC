all: sqrtsum
	cd list; make
	cd prodcons; make
	cd banker; make

LIBS = -pthread

sqrtsum: sqrtsum.o
	gcc -o $@ ${LIBS} sqrtsum.o -lm

clean:
	rm -rf *o sqrtsum
	cd list; make clean
	cd prodcons; make clean
	cd banker; make clean


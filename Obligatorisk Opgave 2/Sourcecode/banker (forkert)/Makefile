all: banker

LIBS = -pthread

banker: banker.o
	gcc -o $@ ${LIBS} banker.o

clean:
	rm -rf *o banker

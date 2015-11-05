all: prodcons

PROCON = prodcons.o list.o
LIBS = -pthread

prodcons: prodcons.o ${PROCON}
	gcc -o $@ ${LIBS} ${PROCON}

clean:
	rm -rf *o prodcons

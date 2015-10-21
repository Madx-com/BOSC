all: prodcons

OBJS = list.o prodcons.o
LIBS = -pthread

prodcons: prodcons.o ${OBJS}
	gcc -o $@ ${LIBS} ${OBJS}

clean:
	rm -rf *o prodcons

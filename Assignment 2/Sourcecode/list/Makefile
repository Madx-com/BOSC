all: fifo prodcons

OBJS = list.o main.o 
OBJS2 = prodcons.o list.o
LIBS = -pthread

fifo: main.o ${OBJS}
	gcc -o $@  ${LIBS} ${OBJS}

prodcons: prodcons.o ${OBJS2}
	gcc -o $@ ${LIBS} ${OBJS2}

clean:
	rm -rf *o fifo prodcons

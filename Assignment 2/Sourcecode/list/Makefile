all: fifo mutexfifo

OBJS = list.o mutexlist.o
OBJS2 = list.o main.o
LIBS = -pthread
LIBS2 = -lpthread

fifo: main.o ${OBJS2}
	gcc -o $@  ${LIBS2} ${OBJS2}

mutexfifo: mutexlist.o ${OBJS}
	gcc -o $@ ${LIBS} ${OBJS}

clean:
	rm -rf *o fifo

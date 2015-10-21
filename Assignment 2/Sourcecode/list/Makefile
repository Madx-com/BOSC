all: fifo mutexfifo

OBJS = mlist.o mutexlist.o
OBJS2 = list.o main.o
LIBS = -pthread

fifo: main.o ${OBJS2}
	gcc -o $@  ${LIBS} ${OBJS2}

mutexfifo: mutexlist.o ${OBJS}
	gcc -o $@ ${LIBS} ${OBJS}

clean:
	rm -rf *o fifo

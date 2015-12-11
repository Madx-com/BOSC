all: fifo

FIFO = list.o main.o 
LIBS = -pthread

fifo: main.o ${FIFO}
	gcc -o $@ ${LIBS} ${FIFO}

clean:
	rm -rf *o fifo

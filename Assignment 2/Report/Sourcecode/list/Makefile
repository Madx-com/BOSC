all: fifo prodcons

FIFO = list.o main.o 
PROCON = prodcons.o list.o
LIBS = -pthread

fifo: main.o ${FIFO}
	gcc -o $@ ${LIBS} ${FIFO}

prodcons: prodcons.o ${PROCON}
	gcc -o $@ ${LIBS} ${PROCON}

clean:
	rm -rf *o fifo prodcons

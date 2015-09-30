all: bosh

OBJS = parser.o print.o forback.o pipe.o bosh.o 
LIBS= -lreadline -ltermcap
CC = gcc

bosh: ${OBJS}
	${CC} -o $@ ${OBJS} ${LIBS}

clean:
	rm -rf *o bosh

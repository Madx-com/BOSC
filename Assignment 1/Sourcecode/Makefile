all: bosh

OBJS = parser.o print.o execmds.o bosh.o pipe.o
LIBS= -lreadline -ltermcap
CC = gcc

bosh: ${OBJS}
	${CC} -o $@ ${OBJS} ${LIBS}

clean:
	rm -rf *o bosh

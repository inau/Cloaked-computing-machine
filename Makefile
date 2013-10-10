all: Sqrt

OBJS = sqrt.o
LIBS= -lm
CC = gcc

Sqrt: sqrt.o ${OBJS}
	${CC} -o $@ sqrt.o ${OBJS} ${LIBS}

clean:
	rm -rf *o Sqrt

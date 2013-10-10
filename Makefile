all: Sqrt

OBJS = 
LIBS= -lpthread -lm
CC = gcc

Sqrt: sqrt.o
	${CC} -o $@ sqrt.o ${LIBS}

clean:
	rm -rf *o Sqrt

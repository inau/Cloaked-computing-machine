all: CShell

OBJS = sqrt.o
LIBS= -lm
CC = gcc

CShell: CShell.o ${OBJS}
	${CC} -o $@ CShell.o ${OBJS} ${LIBS}

clean:
	rm -rf *o CShell

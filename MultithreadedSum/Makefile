all: Sqrt

OBJS = sqrt.o 
LIBS= -lpthread -lm

Sqrt: sqrt.o ${OBJS}
	gcc -o $@ ${OBJS} ${LIBS}

clean:
	rm -rf *o Sqrt

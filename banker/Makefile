all: banker

OBJS = banker.o 
LIBS= -lpthread

banker: ${OBJS}
	gcc -o $@ ${OBJS} ${LIBS}

clean:
	rm -rf *o banker

all: fifo

OBJS = list.o main.o 
LIBS= -lpthread

fifo: main.o ${OBJS}
	gcc -o $@  ${LIBS} ${OBJS}

clean:
	rm -rf *o fifo

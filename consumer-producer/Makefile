all: producer-consumer

OBJS = list.o main.o 
LIBS= -lpthread

producer-consumer: main.o ${OBJS}
	gcc -o $@ ${OBJS} ${LIBS}

clean:
	rm -rf *o producer-consumer

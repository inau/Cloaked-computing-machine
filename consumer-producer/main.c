#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "list.h"
#include <sys/time.h>
#include <semaphore.h>

void *producer(void *param);
void *consumer(void *param);

sem_t mutex, empty, full;

int maxproducts, numproducts = 0, buffersize;

List *list;

int main(int argc, char* argv[])
{
	list = list_new();
	int maxproducers, maxconsumers;
	
	if (argc != 5 ) {
      fprintf(stderr, "must be 4\n");
      return -1;
   }
   
   maxproducers = atoi(argv[1]);
   maxconsumers = atoi(argv[2]);
   buffersize = atoi(argv[3]);
   maxproducts = atoi(argv[4]);
   
   sem_init(&mutex, 0, 1);
   sem_init(&empty, 0, buffersize);
   sem_init(&full, 0, 0);
 
 	/* Make some random shizzle */
 	struct timeval tv;
	gettimeofday(&tv, NULL);
	srand(tv.tv_usec);
 
  
  	int tmp;
  	if(maxproducers >= maxconsumers)
  		tmp = maxproducers;
	else
		tmp = maxconsumers;
		
		
	int counter = 1, t = 0;
	
		//initializing list for pids
	  pthread_t tids[(maxconsumers + maxproducers)];
	  
	  pthread_t tid;
	  pthread_attr_t attr;
	  
	  /* Get default attributes */
	  pthread_attr_init(&attr);
	
   while(counter <= tmp){
   	if(counter <= maxproducers){
   		pthread_create(&tid, &attr, producer, &counter);
      	tids[t++] = tid;
      	printf("\tProducer %d created \n", counter);
   	}
   	if(counter <= maxconsumers){
			pthread_create(&tid, &attr, consumer, &counter);
      	tids[t++] = tid;
      	printf("\tConsumer %d created \n", counter);
   	}
   	Sleep(100);
   	counter++;
   }
   
   //Joining all the tids
  int z;
  for (z = 0; z < (maxconsumers+maxproducers); z++) {
      /* Wait for the given threads to exit */
      pthread_join(tids[z], NULL);
  }
  printf("joints done \n");

  return 0;
   
}

/* Random sleep function */
void Sleep(float wait_time_ms)
{
	wait_time_ms = ((float)rand())*wait_time_ms / (float)RAND_MAX;
	usleep((int) (wait_time_ms * 1e3f)); // convert from ms to us
}


void *producer(void *param) {
	int num;
	num = *((int*)param);
	Sleep(1000);
	Node *n;
	while(numproducts < maxproducts){
		sem_wait(&empty);
		sem_wait(&mutex);
		n = node_new();
		n->elm = malloc(sizeof(int));
		*((int*)n->elm) = numproducts;
		list_add(list, n);
		numproducts++;	
		
		printf("Producer %d produced item %d. \t Items in buffer: %d (out of %d) \n", num, (numproducts-1), list->len, buffersize );
		
		sem_post(&mutex);
		sem_post(&full);		
		
		Sleep(1000);
	}
	printf("\tProducer %d exited\n", num);
	
}

void *consumer(void *param) {
	int num;
	num = *((int*)param);
	Sleep(1000);
	Node *n;
	while(numproducts < (maxproducts) || list->len > 0){
		sem_wait(&full);
		sem_wait(&mutex);
		n = list_remove(list);
		
		printf("Consumer %d consumed item %d. \t Items in buffer: %d (out of %d) \n", num, *((int*)n->elm), list->len, buffersize );
		
		free(n->elm);
		
		sem_post(&mutex);
		sem_post(&empty);
		
		Sleep(1000);		
		
	}
	printf("\tConsumer %d exited\n", num);
}



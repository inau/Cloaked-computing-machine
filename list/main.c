/******************************************************************************
   main.c

   Implementation of a simple FIFO buffer as a linked list defined in list.h.

******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "list.h"

void *runner(void *param);
int num = 0;


// FIFO list;
List *fifo;

int main(int argc, char* argv[])
{
	
	if (argc > 0 && atoi(argv[1]) <= 0) {
      fprintf(stderr, "%d must be > 0\n", atoi(argv[1]));
      return -1;
  }
  
  int nbthreads = atoi(argv[1]); // Get the number of threads...
  
  printf("Number of threads: %d\n", nbthreads);

	// Create list.
	fifo = list_new();

	//initializing list for pids
  pthread_t tids[nbthreads];
  
	pthread_t tid;
  pthread_attr_t attr;
  
  /* Get default attributes */
  pthread_attr_init(&attr);
  
	int i, t = 0;
	for (i = 0; i < nbthreads; i++) {
      /* Create the thread */
      pthread_create(&tid, &attr, runner, NULL);
      tids[t++] = tid;
  }
  
  //Joining all the tids
  int z;
  for (z = 0; z < nbthreads; z++) {
      /* Wait for the given threads to exit */
      pthread_join(tids[z], NULL);
  }
  printf("joins done \n");

  return 0;
}

// Add something to the list and remove something.
void *runner(void *param) {
	Node *n = node_new();
	n->elm = malloc(sizeof(int));
	*((int*)n->elm) = num++;
	list_add(fifo, n);
	
	printf("L77: Going to sleep!\n");
	sleep(1); // Make the thread sleep in order to make way for some other ones.
	printf("L79: Waking up from sleep\n");
	
	Node *n2 = list_remove(fifo);
	if (n2 == NULL) { printf("Error no elements in list\n"); exit(-1);}
	printf("Element: %d\n", *((int*)n2->elm));
	free(n2->elm);
}


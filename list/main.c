/******************************************************************************
   main.c

   Implementation of a simple FIFO buffer as a linked list defined in list.h.

******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "list.h"

void *creator(void *param);
void *destroyer(void *param);
int num = 0;

// FIFO list;
List *fifo;

int main(int argc, char* argv[])
{
	if (argc == 1) {
		fprintf(stderr, "You must give number of threads as input!\n");
		return -1;
	}
	
  if (argc > 0 && atoi(argv[1]) <= 0) {
    fprintf(stderr, "Number of threads must be larger than 0. Given: %d\n", atoi(argv[1]));
    return -1;
  }
  
  // Set the random seed.
  srand(time(NULL));
  
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
  
  int i;
  for (i = 0; i < nbthreads; i++) {
		/* Create the threads! Half create, half destroy! */
		if (i < nbthreads/2) {
			pthread_create(&tid, &attr, creator, NULL);
			tids[i] = tid;
		} else {
			pthread_create(&tid, &attr, destroyer, NULL);
			tids[i] = tid;
		}
  }
  
  //Joining all the tids
  int z;
  for (z = 0; z < nbthreads; z++) {
		/* Wait for the given threads to exit */
		pthread_join(tids[z], NULL);
  }
  printf(" --------\nJoins done \n");
  printf("Elements still in list: %d\n",fifo->len);

  return 0;
}

// Add something to the list.
void *creator(void *param) {
  Node *n = node_new();
  n->elm = malloc(sizeof(int));
  *((int*)n->elm) = num++;
  sleep((rand() % 2) + 1); // Make the thread sleep in order to make way for some other ones.
  printf("Adding Element: %d\n", *((int*)n->elm));
  list_add(fifo, n);

  return 0;
}

// Remove something from the list.
void *destroyer(void *param) {
  sleep((rand() % 2) + 1); // Make the thread sleep in order to make way for some other ones.

  Node *n = list_remove(fifo);
  if (n == NULL) { printf("No elements in list\n"); }
  else {
		printf("Destroying Element: %d\n", *((int*)n->elm));
		free(n->elm);	
  }
  
  return 0;
}


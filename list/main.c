/******************************************************************************
   main.c

   Implementation of a simple FIFO buffer as a linked list defined in list.h.

******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "list.h"

// FIFO list;
List *fifo;

int main(int argc, char* argv[])
{
  fifo = list_new();

  list_add(fifo, node_new_str("s1"));
  list_add(fifo, node_new_str("s2"));

  Node *n1 = list_remove(fifo);
  if (n1 == NULL) { printf("Error no elements in list\n"); exit(-1);}
  Node *n2 = list_remove(fifo);
  if (n2 == NULL) { printf("Error no elements in list\n"); exit(-1);}
  printf("%s\n%s\n", n1->elm, n2->elm);

  return 0;
}


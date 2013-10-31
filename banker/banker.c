#include<stdio.h>
#include<stdlib.h>
#include <sys/time.h>
#include <pthread.h>

typedef struct state {
  int *resource;
  int *available;
  int **max;
  int **allocation;
  int **need;
} State;

// Global variables
int row, col;	// m = row, n = col
State *s = NULL;

// Print available vector.
void printAvailable() {
	int i,j;
	printf("Availability vector:\n");
  for(i = 0; i < col; i++)
    printf("R%d ", i+1);
  printf("\n");
  for(j = 0; j < col; j++)
    printf("%d  ",s->available[j]);
  printf("\n");
}

// Print request vector.
void printRequest(int* request) {
	int i,j;
	printf("Request vector:\n");
  for(i = 0; i < col; i++)
    printf("R%d ", i+1);
  printf("\n");
  for(j = 0; j < col; j++)
    printf("%d  ",request[j]);
  printf("\n");
}

// Mutex for access to state.
pthread_mutex_t state_mutex;

/* Random sleep function */
void Sleep(float wait_time_ms)
{
  // add randomness
  wait_time_ms = ((float)rand())*wait_time_ms / (float)RAND_MAX;
  usleep((int) (wait_time_ms * 1e3f)); // convert from ms to us
}

/* Check if state is safe */
int stateSafeCheck() {
	int *work = (int *) malloc(col*sizeof(int));
  int *finish = (int *) malloc(row*sizeof(int));
  
  int i, j, l;
  l = col > row ? col : row;
  // Step 1.
  for (i = 0; i < l; i++) {
  	if (i < row)
  	{
  		finish[i] = 0;
  	}
  	if (i < col)
  	{
  		work[i] = s->available[i];
  	}
  }
  
  for (i = 0; i < row; i++) {
  	for (j = 0; j < col; j++)
  	{
  		// Step 2.
  		if (finish[i] == 0 && s->need[i][j] <= work[j]) {
  			// Step 3.
  			work[j] = work[j] + s->allocation[i][j];
  			finish[i] = 1;
  		} else {
  			break; // Goto step 4 (not safe).
  		}
  	}
  }
  
  // Step 4
  // If finish is true in all instances, the state is safe.
  for (i = 0; i < row; i++)
  {
  	if (finish[i] == 0)
  	{
  		printf("State is not safe!\n");
  		return 0;
  	}
  }
  
  printf("State is safe!\n");
  return 1;
  
}

/* Allocate resources in request for process i, only if it 
   results in a safe state and return 1, else return 0 */
int resource_request(int i, int *request)
{

	printRequest(request);
	if (stateSafeCheck() == 0)
	{
		return 0;
	}
	int j;
	for (j = 0; j < col; j++) {
		// Step 1.
		if (request[j] > s->need[i][j]) {
			//ERROR
			printf("Request can't be granted, because the process has exceeded its maximum claim\n");
			return 0;
		}
	}
	
	for (j = 0; j < col; j++) {
		// Step 2.
		while (request[j] > s->available[j]) {
			//ERROR
			printf("No resources are available, therefore the process must wait!\n");
			sleep(100);
		}
		
		// Step 3.
		s->available[j] -= request[j];
		s->allocation[i][j] -= request[j];
		s->need[i][j] -= request[j];
	}
	
	printAvailable();
	
	// Check if the resulting state is safe, return 1 if true, else 0.
	
  return 1;
}

/* Release the resources in request for process i */
void resource_release(int i, int *request)
{
	printf("RELEASING RESOURCES!\n");
	printRequest(request);
	int j;
	for (j = 0; j < col; j++) {
		s->available[j] -= request[j];
		s->allocation[i][j] -= request[j];
		s->need[i][j] -= request[j];
	}
	
	printAvailable();
	
}

/* Generate a request vector */
void generate_request(int i, int *request)
{
  int j, sum = 0;
  while (!sum) {
    for (j = 0;j < col; j++) {
      request[j] = s->need[i][j] * ((double)rand())/ (double)RAND_MAX;
      sum += request[j];
    }
  }
  printf("Process %d: Requesting resources.\n",i);
}

/* Generate a release vector */
void generate_release(int i, int *request)
{
  int j, sum = 0;
  while (!sum) {
    for (j = 0;j < col; j++) {
      request[j] = s->allocation[i][j] * ((double)rand())/ (double)RAND_MAX;
      sum += request[j];
    }
  }
  printf("Process %d: Releasing resources.\n",i);
}

/* Threads starts here */
void *process_thread(void *param)
{
  /* Process number */
  int i = (int) (long) param, j;
  /* Allocate request vector */
  int *request = malloc(col*sizeof(int));
  while (1) {
    /* Generate request */
    generate_request(i, request);
    while (!resource_request(i, request)) {
      /* Wait */
      Sleep(100);
    }
    /* Generate release */
    generate_release(i, request);
    /* Release resources */
    resource_release(i, request);
    /* Wait */
    Sleep(1000);
  }
  free(request);
}

int main(int argc, char* argv[])
{
  /* Get size of current state as input */
  int i, j;
  printf("Number of processes: \n");
  scanf("%d", &row);
  printf("Number of resources: \n");
  scanf("%d", &col);

	printf("\tFINISHED LOADING IN NB PROCESSES AND RESOURCES\n");

  /* Allocate memory for state */
  s = (State *) malloc(sizeof(State));
  s->resource 	= (int *) malloc(col*sizeof(int));
  s->available	= (int *) malloc(col*sizeof(int));
  s->max 				= (int **) malloc(row*sizeof(int));
  s->allocation = (int **) malloc(row*sizeof(int));
  s->need 			= (int **) malloc(row*sizeof(int));
  if (s == NULL) { printf("\nYou need to allocate memory for the state!\n"); exit(0); };

	printf("\tFINISHED ALLOCATING SPACE FOR STATE\n");

  /* Get current state as input */
  printf("Resource vector: \n");
  for(i = 0; i < col; i++)
    scanf("%d", &s->resource[i]);
  printf("Enter max matrix: \n");
  for(i = 0;i < row; i++) {
  	// Allocate the row
  	s->max[i] = (int *) malloc(col*sizeof(int));
    for(j = 0;j < col; j++) {
      	scanf("%d", &s->max[i][j]);
      }
    }
  printf("Enter allocation matrix: \n");
  for(i = 0; i < row; i++) {
  	// Allocate the row
  	s->allocation[i] = (int *) malloc(col*sizeof(int));
    for(j = 0; j < col; j++) {
      scanf("%d", &s->allocation[i][j]);
    }
  }
  printf("\n");
  
  printf("\tFINISHED LOADING VECTORS AND MATRIXES\n");

  /* Calcuate the need matrix */
  for(i = 0; i < row; i++) {
    // Allocate the row
  	s->need[i] = (int *) malloc(col*sizeof(int));
    for(j = 0; j < col; j++) {
      s->need[i][j] = s->max[i][j] - s->allocation[i][j];
    }
  }

  /* Calcuate the availability vector */
  for(j = 0; j < col; j++) {
    int sum = 0;
    for(i = 0; i < row; i++)
      sum += s->allocation[i][j];
    s->available[j] = s->resource[j] - sum;
  }

  /* Output need matrix and availability vector */
  printf("Need matrix:\n");
  for(i = 0; i < col; i++)
    printf("R%d ", i+1);
  printf("\n");
  for(i = 0; i < row; i++) {
    for(j = 0; j < col; j++)
      printf("%d  ",s->need[i][j]);
    printf("\n");
  }
  printAvailable();

  /* If initial state is unsafe then terminate with error */
  int safe = stateSafeCheck();
	if (safe == 0)
	{
		return 1;
	}

  /* Seed the random number generator */
  struct timeval tv;
  gettimeofday(&tv, NULL);
  srand(tv.tv_usec);
  
  /* Create row threads */
  pthread_t *tid = malloc(row*sizeof(pthread_t));
  for (i = 0; i < row; i++)
    pthread_create(&tid[i], NULL, process_thread, (void *) (long) i);
  
  /* Wait for threads to finish */
  pthread_exit(0);
  free(tid);

  /* Free state memory */
  free(s->resource);
  free(s->available);
  free(s->max);
  free(s->allocation);
  free(s->need);
  free(s);
}

#include <pthread.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

double sum;
void *runner(void *param);

typedef struct workload {
    int _begin;
    int _end;
    double _result;
} Workload;

int main(int argc, char *argv[]) {
    clock_t t1, t2; // Timers.
    pthread_t tid;
    pthread_attr_t attr;

    if (argc < 2) {
        fprintf(stderr, "usage: a.out <integer value>\n");
        return -1;
    }

    if (atoi(argv[1]) < 0) {
        fprintf(stderr, "%d must be >= 0\n", atoi(argv[1]));
        return -1;
    }

    if (argc > 2 && atoi(argv[2]) <= 0) {
        fprintf(stderr, "%d must assign > 0 threads \n", atoi(argv[2]));
        return -1;
    }
    int MAXPROCESSES = atoi(argv[2]);
    int MAXCALCS = atoi(argv[1]);
    
    if (MAXPROCESSES > MAXCALCS) {
    	MAXPROCESSES = MAXCALCS;
    	fprintf(stderr, "Too many threads according to calculations. Threads in use will be: %d\n", MAXCALCS);
    }

    sum = 0;

    //initializing list for pids
    pthread_t tids[MAXPROCESSES];

    /* Generate the workload structs for each processor*/
    Workload * wlds[MAXPROCESSES];

    int z;
    Workload *w;
    for (z = 0; z < MAXPROCESSES; z++) {
        w = (Workload*) malloc(sizeof (Workload));
        wlds[z] = w;
    }

    /* Get default attributes */
    pthread_attr_init(&attr);

    
    int t = 0, i = 0;

		t1 = clock(); // Get time.

	
		for (i; i < MAXPROCESSES; i++) {
			
			int s = (int)(((float) MAXCALCS / (float) MAXPROCESSES)*(float)(i)+1.0);
			int e = (int)(((float) MAXCALCS / (float) MAXPROCESSES)*(float)(i + 1));
			printf("BlockSize for thread %d is %d\n", i, e-s+1);
			
			wlds[i]->_begin = s;
			wlds[i]->_end = e;
			
			/* Create the thread */
			pthread_create(&tid, &attr, runner, wlds[t]);
      tids[t++] = tid;
			printf("T %d Id %d \n", t, (int) tid);
		}

    //Joining all the tids
    for (z = 0; z < MAXPROCESSES; z++) {
        /* Wait for the given threads to exit */
        pthread_join(tids[z], NULL);
    }
    printf("joins done \n");

    /* Summing all the partial results*/
    for (z = 0; z < MAXPROCESSES; z++) {
        sum += wlds[z]->_result;
    }
    
    t2 = clock(); // Get time again.

    free(w);

    printf("sum = %f\n", sum);
    
    float diff = ((float)t2 - (float)t1)/CLOCKS_PER_SEC; // Get the difference between the timers.
    printf("Time spent %f seconds \n", diff);
}

/* The thread will begin control in this function */
void *runner(void *param) {
    int i = ((Workload*) param)->_begin;

    int upper = ((Workload*) param)->_end;

    ((Workload*) param)->_result = 0;

    for (i; i <= upper; i++)
        ((Workload*) param)->_result += sqrt(i);

    printf("RUNNER [ Start: %d || End: %d || Result: %f ] \n", ((Workload*) param)->_begin, upper, ((Workload*) param)->_result);

    pthread_exit(0);
}


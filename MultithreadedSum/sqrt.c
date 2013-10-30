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
    int start = time(NULL);
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

    int blocksize = (int) MAXCALCS / MAXPROCESSES;
    printf("BlockSize per thread %d \n", blocksize);
    int t = 0, i = 1;

    for (i; i < MAXCALCS + 1; i = i + blocksize) {
        wlds[t]->_begin = i;
        if (i + blocksize >= MAXCALCS) wlds[t]->_end = MAXCALCS + 1;
        else {
            wlds[t]->_end = i + blocksize;
        }

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

    free(w);

    printf("sum = %f\n", sum);

    int end = time(NULL);
    printf("Time spent %d \n", (end - start));
}

/* The thread will begin control in this function */
void *runner(void *param) {
    int i = ((Workload*) param)->_begin;

    int upper = ((Workload*) param)->_end;

    ((Workload*) param)->_result = 0;

    for (i; i < upper; i++)
        ((Workload*) param)->_result += sqrt(i);

    printf("RUNNER [ Start: %d || End: %d || Result: %f ] \n", ((Workload*) param)->_begin, upper, ((Workload*) param)->_result);

    pthread_exit(0);
}

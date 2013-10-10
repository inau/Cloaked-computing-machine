#include <pthread.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

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

    pthread_t tids[MAXPROCESSES];
    
    /* Get default attributes */
    pthread_attr_init(&attr);
    int blocksize;
    
    if (MAXCALCS % MAXPROCESSES != 0) {
         blocksize = MAXCALCS / MAXPROCESSES;
    }

    else {
        blocksize = MAXCALCS / MAXPROCESSES;
        Workload *w = NULL;
        int t = 0, i = 0;
        for (i; i < MAXPROCESSES; i + blocksize) {
            w->_begin = i;
            w->_end = i + blocksize;
            /* Create the thread */
            pthread_create(&tid, &attr, runner, (void *)w);
            tids[t++] = tid;
        }



        /* Wait for the thread to exit */
        pthread_join(tid, NULL);

        printf("sum = %f\n", sum);

        int end = time(NULL);
        printf("Time spent %d \n", (end - start));
    }

    /* The thread will begin control in this function */
    void *runner(void *param) {
        int i = (Workload)param->_begin;
        int upper = (Workload)param->_end;
        (Workload)param->_result = 0;

        for (i = 1; i <= upper; i++)
            (Workload)param->_result += sqrt(i);

        pthread_exit(0);
    }

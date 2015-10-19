#include <pthread.h>
#include <stdio.h>
#include <math.h>

typedef struct 
{
	int n;
	double sum;
} SQRTSUM;

SQRTSUM sqrtsum; /* globally shared struct */
pthread_mutex_t mutex_sqrtsum; /* PThread mutex variable */

void *runner(void *param); /* threads call this function */

/* Takes 2 arguments, a number N for the summation limit and t as in number of threads */
int main(int argc, char *argv[])
{
	if (atoi(argv[1]) < 0)
	{
		fprintf(stderr, "%d must be over >= 0\n", atoi(argv[1]));
		return -1;
	}

	int i, NUM_THREADS = atoi(argv[2]);
	pthread_t tid[NUM_THREADS]; /* the thread identifiers */
	pthread_attr_t attr; /* set of thread attributes */

	int n = atoi(argv[1])/NUM_THREADS;
	sqrtsum.n = n;

	pthread_mutex_init(&mutex_sqrtsum, NULL);

	/* get the default attributes */
	pthread_attr_init(&attr);
	/* set the attribute as joinable, so the threads can join with the main thread */
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	
	/* create the threads */
	for (i = 0; i < NUM_THREADS; i++)
	{
		pthread_create(&tid[i], &attr, runner, (void *)i);
	}
	/* wait for the thread to exit */
	for (i=0; i < NUM_THREADS; i++)
	{
		pthread_join(tid[i],NULL);
	}

	printf("sqrtsum = %f\n", sqrtsum.sum);
}

/* Threads will use this function */
void *runner(void *param)
{
	int i, n, start, tid, upper;	
	
	double lsqrtsum = 0.0; /* local sum variable */
	n = sqrtsum.n; /* short summation limit */
	tid = (int)param; /* thread id */
	printf("Thread id: %d\n", tid);
	start = n * tid; /* start value */	
	if(start != 0)
	{
		start += 1;
		upper = start + n - 1;
	}
	else
	{
		upper = start + n;
	}
	printf("%d. Start value: %d\n", tid, start);
	printf("%d. Upper value: %d\n", tid, upper);
	
	for (i = start; i <= upper ; i++)
	{
		lsqrtsum += sqrt(i);
	}

	pthread_mutex_lock(&mutex_sqrtsum);	/* lock mutex */
	sqrtsum.sum += lsqrtsum; /* update global struct variable */
	pthread_mutex_unlock(&mutex_sqrtsum); /* unlock mutex */

	pthread_exit(0); /* exit pthread */
}

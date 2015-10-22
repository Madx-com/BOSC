#include <pthread.h>
#include <stdio.h>
#include <math.h>
#include <sys/time.h>

typedef struct
{
	int n;
	double sum;
} SQRTSUM;

// globally shared struct
SQRTSUM sqrtsum; 

// PThread mutex variable
pthread_mutex_t mutex_sqrtsum; 

// structs to determine time
struct timeval tp1, tp2;
struct timezone tpz1, tpz2;

// threads call this function
void *runner(void *param);

/* Takes 2 arguments, a number N for the summation limit and t as in number of threads */
int main(int argc, char *argv[])
{
	gettimeofday(&tp1, &tpz1); 

	if (atoi(argv[1]) < 0)
	{
		fprintf(stderr, "%d must be over >= 0\n", atoi(argv[1]));
		return -1;
	}

	int i, NUM_THREADS = atoi(argv[2]);
	// the thread id array
	pthread_t tid[NUM_THREADS];
	// set of thread attributes
	pthread_attr_t attr;
	// assumption: argv[1] % NUM_THREADS = 0
	int n = atoi(argv[1])/NUM_THREADS;
	sqrtsum.n = n;

	pthread_mutex_init(&mutex_sqrtsum, NULL);

	// get the default attributes 
	pthread_attr_init(&attr);
	// set the attribute as joinable, so the threads can join with the main thread
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	// create the threads
	for (i = 0; i < NUM_THREADS; i++)
	{
		pthread_create(&tid[i], &attr, runner, (void *)i);
	}

	// destroy attribute
	pthread_attr_destroy(&attr);

	// wait for the thread to exit
	for (i=0; i < NUM_THREADS; i++)
	{
		pthread_join(tid[i],NULL);
	}
	printf("sqrtsum = %f\n", sqrtsum.sum);

	// destroy mutex
	pthread_mutex_destroy(&mutex_sqrtsum);

	gettimeofday(&tp2, &tpz2);

	// calculate program time
	int time = (tp2.tv_sec - tp1.tv_sec) * 1000 + (tp2.tv_usec - tp1.tv_usec) / 1000;
	printf("Total time(ms): %d\n", time);
	time;
	return 0;
}

/* Threads will use this function */
void *runner(void *param)
{
	int i, n, start, tid, upper;

	// local sum variable
	double lsqrtsum = 0.0;
	// short summation limit	
	n = sqrtsum.n;
	// thread id
	tid = (int *)param;
	// start value for loop
	start = n * tid + 1;
	printf("Thread %d: Start value: %d\n", tid, start);
	// upper value for loop
	upper = start + n;
	printf("Thread %d: Upper value: %d\n", tid, upper);

	for (i = start; i < upper ; i++)
	{
		lsqrtsum += sqrt(i);
	}

	printf("Thread %d: Local sqrtsum: %f\n", tid, lsqrtsum);

	// lock mutex
	pthread_mutex_lock(&mutex_sqrtsum);
	// update global struct variable
	sqrtsum.sum += lsqrtsum;
	// unlock mutex
	pthread_mutex_unlock(&mutex_sqrtsum);
	// exit pthread
	pthread_exit(0);
}

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <sys/time.h>
#include "mlist.h"

typedef struct pc {
	List *l;
	sem_t full;
	sem_t empty;
	sem_t mutex;
} PC;

int p;
int c;

PC prodcons;

int PRODUCERS;
int CONSUMERS;
int BUFFSIZE;
int PRODUCTS_IN_TOTAL;

void *producer(void *param);
void *consumer(void *param);
void Sleep(float wait_time_ms);

struct timeval tv;

int main(int argc, char *argv[])
{
	gettimeofday(&tv, NULL);
	srand(tv.tv_usec);
	
	PRODUCERS = atoi(argv[1]);
	CONSUMERS = atoi(argv[2]);
	BUFFSIZE = atoi(argv[3]);
	PRODUCTS_IN_TOTAL = atoi(argv[4]);

	int i;
	pthread_attr_t attr;

	sem_init(&prodcons.full, 0, 0);
	sem_init(&prodcons.empty, 0, BUFFSIZE);
	sem_init(&prodcons.mutex, 0, 1);

	prodcons.l = list_new();

	/* producer id's */
	pthread_t pid[PRODUCERS];

	/* consumer id's */
	pthread_t cid[CONSUMERS];

	/* Initialize thread attributes */
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	int num_iter = PRODUCTS_IN_TOTAL / PRODUCERS;
	int remainder = PRODUCTS_IN_TOTAL % PRODUCERS;

	/* create the producers */
	for(i = 0; i < PRODUCERS; i++)
	{	
		if((i+1) == PRODUCERS)
		{
			num_iter = num_iter+remainder;
		}
		pthread_create(&pid[i], &attr, producer, (void *)num_iter);
	}
	
	/* create the consumers */
	for(i = 0; i < CONSUMERS; i++)
	{
		pthread_create(&cid[i], &attr, consumer, NULL);
	}

	/* wait for the producer threads */
	for(i = 0; i < PRODUCERS; i++)
	{
		pthread_join(pid[i], NULL);
	}
	
	/* wait for the consumer threads */
	for(i = 0; i < CONSUMERS; i++)
	{
		pthread_join(cid[i], NULL);
	}

	return 0;
}

void *producer(void *param)
{
	int i, iter;
	iter = (int *)param;
	
	for(i = 0; i < iter; i++)
	{
		if(p == PRODUCTS_IN_TOTAL)
		{
			break;
		}
		Node *n = node_new_str("test_object");
		/* wait till empty is decremented */
		wait(&prodcons.empty);
		/* wait for mutex */
		wait(&prodcons.mutex);
		/* produce product */
		list_add(prodcons.l, n);
		p += 1;
		printf("Produced Item %d. Items in buffer %d (out of %d)\n", p, prodcons.l->len, BUFFSIZE);
		fflush(stdout);
		/* release the mutex */		
		sem_post(&prodcons.mutex);
		/* notify waiting consumer threads that a space is filled */	
		sem_post(&prodcons.full);
		Sleep(2000);
	}
}

void *consumer(void *param)
{
	do	
	{
		/* wait till full is incremented */
		wait(&prodcons.full);
		/* wait for mutex */
		wait(&prodcons.mutex);
		/* consume */	
		Node *n = node_new();	
		n = list_remove(prodcons.l);
		if(n->elm != NULL)
		{
			c += 1;
			printf("Consumed Item: %d. Items in buffer %d (out of %d)\n", c, prodcons.l->len, BUFFSIZE);
			fflush(stdout);
		}
		/* release the mutex */
		sem_post(&prodcons.mutex);
		/* notify waiting producer threads that a space is free */
		sem_post(&prodcons.empty);
		Sleep(2000);
	} while(c != PRODUCTS_IN_TOTAL)	;
}

void Sleep(float wait_time_ms)
{
	wait_time_ms = ((float)rand()) * wait_time_ms / (float)RAND_MAX;
	usleep((int) (wait_time_ms * 1e3f));
}

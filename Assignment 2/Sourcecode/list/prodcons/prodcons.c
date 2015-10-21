#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <sys/time.h>
#include "list.h"

typedef struct pc {
	List *l;
	int produced;
	int consumed;
	sem_t full;
	sem_t empty;
	sem_t mutex;
} PC;

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

	for(i = 0; i < PRODUCERS; i++)
	{	
		printf("%d. Prod ForLoop\n", i);
		if((i+1) == PRODUCERS)
		{
			num_iter = num_iter+remainder;
		}
		pthread_create(&pid[i], &attr, producer, (void *)num_iter);
	}
	
	for(i = 0; i < CONSUMERS; i++)
	{
		printf("%d. Cons ForLoop\n", i);
		pthread_create(&cid[i], &attr, consumer, NULL);
	}

	for(i = 0; i < PRODUCERS; i++)
	{
		pthread_join(pid[i], NULL);
	}
	
	for(i = 0; i < CONSUMERS; i++)
	{
		pthread_join(cid[i], NULL);
	}

	return 0;
}

void *producer(void *param)
{
	printf("Producer\n");
	int i, iter;
	iter = (int *)param;
	
	for(i = 0; i < iter; i++)
	{
		Node *n = node_new_str("test_object");
		/* wait till empty is decremented */
		wait(&prodcons.empty);
		/* wait for mutex */
		wait(&prodcons.mutex);
		/* produce product */
		list_add(prodcons.l, n);
		prodcons.produced += 1;
		printf("%d. Produced product %s. Items in buffer %d (out of %d)\n", i, n->elm, prodcons.l->len, BUFFSIZE);
		/* release the mutex */		
		sem_post(&prodcons.mutex);		
		/* notify waiting consumer threads that a space is filled */	
		sem_post(&prodcons.full);
		Sleep(1000);	
	}
}

void *consumer(void *param)
{
	printf("Consumer\n");
	do	
	{
		/* wait till full is incremented */
		wait(&prodcons.full);
		/* wait for mutex */
		wait(&prodcons.mutex);
		/* consume */	
		Node *n;	
		n = list_remove(prodcons.l);
		prodcons.consumed += 1;
		printf("Consumed Item: %s. Items in buffer %d (out of %d)\n", n->elm, prodcons.l->len, BUFFSIZE); 
		/* release the mutex */
		sem_post(&prodcons.mutex);
		/* notify waiting producer threads that a space is free */
		sem_post(&prodcons.empty);
		Sleep(1000);	
	} while(prodcons.consumed != PRODUCTS_IN_TOTAL)	;
}

void Sleep(float wait_time_ms)
{
	wait_time_ms = ((float)rand()) * wait_time_ms / (float)RAND_MAX;
	usleep((int) (wait_time_ms * 1e3f));
}

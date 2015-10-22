#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <sys/time.h>
#include "list.h"

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

	p = 0;
	c = 0;

	prodcons.l = list_new();

	/* producer id's */
	pthread_t pid[PRODUCERS];

	/* consumer id's */
	pthread_t cid[CONSUMERS];

	/* Initialize thread attributes */
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	/* create the producers */
	for(i = 0; i < PRODUCERS; i++)
	{	
		pthread_create(&pid[i], &attr, producer, (void *)i);
	}
	
	/* create the consumers */
	for(i = 0; i < CONSUMERS; i++)
	{
		pthread_create(&cid[i], &attr, consumer, (void *)i);
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

	if(c == p)
	{
		printf("Success! All products produced and consumed.\n");
	}
	else
	{
		printf("All products not consumed/produced.\n");
	}

	return 0;
}

void *producer(void *param)
{
	int id = (int *)param;
	Node *n;

	while(p < PRODUCTS_IN_TOTAL)
	{
		/* wait till empty is decremented */
		sem_wait(&prodcons.empty);
		/* wait for mutex */
		sem_wait(&prodcons.mutex);
		/* produce product */
		char str[10];
		sprintf(str, "P%d", (p+1));
		n = node_new_str(str);
		list_add(prodcons.l, n);
		p += 1;
		/* release the mutex */		
		sem_post(&prodcons.mutex);
		/* notify waiting consumer threads that a space is filled */	
		sem_post(&prodcons.full);
		printf("%d. Produced Item %d: %s. Items in buffer %d (out of %d)\n", id, p, (char *)n->elm, prodcons.l->len, BUFFSIZE);
		fflush(stdout);		
		Sleep(2000);
	}
}

void *consumer(void *param)
{
	int id = (int *)param;
	Node *n;

	while(c < PRODUCTS_IN_TOTAL)		
	{
		/* wait till full is incremented */
		sem_wait(&prodcons.full);
		/* wait for mutex */
		sem_wait(&prodcons.mutex);
		/* consume */		
		n = list_remove(prodcons.l);
		c += 1;
		/* release the mutex */
		sem_post(&prodcons.mutex);
		/* notify waiting producer threads that a space is free */
		sem_post(&prodcons.empty);
		printf("%d. Consumed Item %d: %s. Items in buffer %d (out of %d)\n", id, c, (char *)n->elm, prodcons.l->len, BUFFSIZE);
		fflush(stdout);		
		Sleep(2000);
	}
}

void Sleep(float wait_time_ms)
{
	wait_time_ms = ((float)rand()) * wait_time_ms / (float)RAND_MAX;
	usleep((int) (wait_time_ms * 1e3f));
}

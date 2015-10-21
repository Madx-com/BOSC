/******************************************************************************
   main.c

   Implementation of a simple FIFO buffer as a linked list defined in list.h.

******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include "list.h"

/* mutex */
pthread_mutex_t mtx;

/* global variable */
List *fifo;

/* thread functions */ 
void *thread_add(void *param);
void *thread_remove(void *param);

int main(int argc, char* argv[])
{
	int i;	
	int NUM_THREADS = 4;
	
	pthread_t tid[NUM_THREADS];
	pthread_attr_t attr;

	fifo = list_new();

	/* Initialize mutex */
	pthread_mutex_init(&mtx, NULL);

	/* Initialize and set state for attribute */
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	
	Node *n[4];
	n[0] = node_new_str("s1");
	n[1] = node_new_str("s2");
	n[2] = node_new_str("s3");
	n[3] = node_new_str("s4");

	int len = NUM_THREADS / 4;
	/* create the threads */
	for(i = 0; i < 4; i++)
	{
		pthread_create(&tid[i], &attr, thread_add, n[i]);
	}
		
	for(i = 0; i < 4; i++)
	{
		pthread_join(tid[i], NULL);
	}
	
	for(i = 0; i < 6; i++)
	{
		pthread_create(&tid[i], &attr, thread_remove, NULL);
	}

	for(i = 0; i < 6; i++)
	{
		pthread_join(tid[i], NULL);
	}

	return 0;
}

void *thread_add(void *param)
{
	Node *n = (Node *)param;
	list_add(fifo, n);
}

void *thread_remove(void *param)
{
	Node *n;
	n = list_remove(fifo);
}



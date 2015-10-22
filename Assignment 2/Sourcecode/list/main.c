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
	int ADDS = atoi(argv[1]);
	int REMOVES = atoi(argv[2]);
	
	pthread_t aid[ADDS]; // Add threads
	pthread_t rid[REMOVES];		// Remove threads
	pthread_attr_t attr;

	fifo = list_new();

	/* Initialize and set state for attribute */
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	/* create the add threads */
	for(i = 0; i < ADDS; i++)
	{
		pthread_create(&aid[i], &attr, thread_add, (void *)i);
	}
		
	for(i = 0; i < ADDS; i++)
	{
		pthread_join(aid[i], NULL);
	}

	/* create the remove threads */	
	for(i = 0; i < REMOVES; i++)
	{
		pthread_create(&rid[i], &attr, thread_remove, NULL);
	}

	for(i = 0; i < REMOVES; i++)
	{
		pthread_join(rid[i], NULL);
	}

	int diff = ADDS - REMOVES;
	if((diff <= 0) && (fifo->len == 0))
	{
		printf("Success! List is correct. Diff:%d Length:%d\n", diff, fifo->len);
	}
	else if((diff > 0) && (fifo->len == diff))
	{
		printf("Success! List is correct. Diff:%d Length:%d\n", diff, fifo->len);
	}
	else
	{
		printf("List is flawed. Diff:%d Length:%d\n", diff, fifo->len);
	}

	return 0;
}

void *thread_add(void *param)
{
	int id = (int *)param;

	char str[10];
	sprintf(str, "P%d", id);
	Node *n = node_new_str(str);
	list_add(fifo, n);
}

void *thread_remove(void *param)
{
	Node *n = (Node *) malloc(sizeof(Node));
	n = list_remove(fifo);
}



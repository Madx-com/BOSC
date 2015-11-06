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
	
	// Add threads
	pthread_t aid[ADDS];
	// Remove threads
	pthread_t rid[REMOVES];
	pthread_attr_t attr;

	// create list
	fifo = list_new();

	// Initialize and set state for attribute
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	// create the add threads
	for(i = 0; i < ADDS; i++)
	{
		pthread_create(&aid[i], &attr, thread_add, (void *) (long) i);
	}
		
	for(i = 0; i < ADDS; i++)
	{
		pthread_join(aid[i], NULL);
	}

	// create the remove threads
	for(i = 0; i < REMOVES; i++)
	{
		pthread_create(&rid[i], &attr, thread_remove, NULL);
	}

	// destroy attributes
	pthread_attr_destroy(&attr);

	for(i = 0; i < REMOVES; i++)
	{
		pthread_join(rid[i], NULL);
	}

	// check credibility of the list
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

/* Adds a node to the list */
void *thread_add(void *param)
{
	int id = (int) (long) param;

	char str[10];
	sprintf(str, "P%d", id);
	Node *n = node_new_str(str);
	list_add(fifo, n);
}

/* Removes node from the list */
void *thread_remove(void *param)
{
	Node *n = (Node *) malloc(sizeof(Node));
	n = list_remove(fifo);
}



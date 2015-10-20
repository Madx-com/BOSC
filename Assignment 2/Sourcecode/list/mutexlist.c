/******************************************************************************
   main.c

   Implementation of a simple FIFO buffer as a linked list defined in list.h.

******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "list.h"

// FIFO list
List *fifo;

/* mutex */
pthread_mutex_t mtx;

/* thread functions */ 
void *mlist_add(void *param);
void *mlist_remove(void *param);

int main(int argc, char* argv[])
{
	int i;	
	int NUM_THREADS = atoi(argv[1]);
	
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
		printf("In for loop...\n");
		pthread_create(&tid[i], &attr, mlist_add, n[i]);
	}

	for(i = 0; i < 4; i++)
	{
		pthread_join(tid[i], NULL);
	}
	
	Node *n1 = list_remove(fifo);
	if (n1 == NULL) { printf("Error no elements in list\n"); exit(-1);}
	Node *n2 = list_remove(fifo);
	if (n2 == NULL) { printf("Error no elements in list\n"); exit(-1);}
	printf("%s\n%s\n", n1->elm, n2->elm);
	
	return 0;
}

void *mlist_add(void *param)
{
	Node *n = (Node *)param;
	pthread_mutex_lock(&mtx);
	printf("Adding %s...\n",n->elm);
	list_add(fifo, n);
	pthread_mutex_unlock(&mtx);
}

void *mlist_remove(void *param)
{
	pthread_mutex_lock(&mtx);
	printf("Removing...\n");
	param = list_remove(fifo);
	pthread_mutex_unlock(&mtx);
}



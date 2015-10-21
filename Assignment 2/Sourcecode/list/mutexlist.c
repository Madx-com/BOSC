/******************************************************************************
   mutexlist.c

   Test program for mlist.c

******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "list.h"

/* FIFO list */
List *fifo;

int main(int argc, char* argv[])
{
	int i;

	fifo = list_new();	
	
	list_add(fifo, node_new_str("s1"));
	list_add(fifo, node_new_str("s2"));
	list_add(fifo, node_new_str("s3"));
	list_add(fifo, node_new_str("s4"));
	list_add(fifo, node_new_str("s5"));
	list_add(fifo, node_new_str("s6"));
	list_add(fifo, node_new_str("s7"));
	list_add(fifo, node_new_str("s8"));
	list_add(fifo, node_new_str("s9"));
	list_add(fifo, node_new_str("s10"));
		
	for(i = 0; i < 12; i++)
	{
		Node *n1 = list_remove(fifo);
		printf("Removed: %s\n", n1->elm);		
	}

	return 0;
}

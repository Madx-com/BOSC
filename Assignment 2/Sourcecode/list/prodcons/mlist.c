/******************************************************************************
   mlist.c

   Implementation of thread safe linked list defined in mlist.h.

******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "mlist.h"

pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

/* list_new: return a new list structure */
List *list_new(void)
{
	List *l;

	l = (List *) malloc(sizeof(List));
	l->len = 0;

	/* insert root element which should never be removed */
	l->first = l->last = (Node *) malloc(sizeof(Node));
	l->first->elm = NULL;
	l->first->next = NULL;
	return l;
}

/* list_add: add node n to list l as the last element */
void *mlist_add(void *param)
{
	MList *ml = (MList *)param;

	/* trying to lock mutex */
	pthread_mutex_trylock(&mtx);

	ml->l->last = ml->l->last->next = ml->n;
	ml->l->len += 1;	

	/* unlocking mutex */
	pthread_mutex_unlock(&mtx);			
}

/* list_remove: remove and return the first (non-root) element from list l */
void *mlist_remove(void *param)
{
	MList *ml = (MList *)param;	

	/* trying to lock mutex */
	pthread_mutex_lock(&mtx);

	if(ml->l->len == 0)
	{
		ml->n = node_new();
		pthread_mutex_unlock(&mtx);
		pthread_exit(NULL);
	}

	ml->n = ml->l->first->next;

	if(ml->n->next)
	{
		ml->l->first->next = ml->n->next;
	}

	ml->l->len -= 1;

	if(ml->l->len == 0)
	{
		ml->l->last = node_new();
	}

	/* unlocking mutex */
	pthread_mutex_unlock(&mtx);
}

/* node_new: return a new node structure */
Node *node_new(void)
{
	Node *n;
	n = (Node *) malloc(sizeof(Node));
	n->elm = NULL;
	n->next = NULL;
	return n;
}

/* node_new_str: return a new node structure, where elm points to new copy of s */
Node *node_new_str(char *s)
{
	Node *n;
	n = (Node *) malloc(sizeof(Node));
	n->elm = (void *) malloc((strlen(s)+1) * sizeof(char));
	strcpy((char *) n->elm, s);
	n->next = NULL;
	return n;
}

void list_add(List *l, Node *n)
{
	pthread_attr_t attr;
	pthread_t tid;

	/* MList */
	MList *ml;
	ml = (MList *) malloc(sizeof(MList));
	ml->n = n;	
	ml->l = l;		

	/* Initialize attribute */
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	pthread_create(&tid, &attr, mlist_add, ml);

	pthread_join(tid, NULL);
}

Node *list_remove(List *l)
{
	pthread_attr_t attr;
	pthread_t tid;
	Node *n;

	/* MList */
	MList *ml;
	ml = (MList *) malloc(sizeof(MList));
	ml->n = n;	
	ml->l = l;

	/* Initialize and set state for attribute */
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	pthread_create(&tid, &attr, mlist_remove, ml);

	pthread_join(tid, NULL);

	return ml->n;
}

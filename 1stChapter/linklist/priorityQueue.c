#include<stdio.h>
#include<string.h>
#include<stdlib.h>

typedef struct node{
	struct node* next;
	int priority;
	int val;
}NODE;

int enqueue(NODE **queue, NODE* p){
	NODE *q = *queue;
	if(q==0 || p->priority > q->priority){
		*queue = p;
		p->next = q;
	}
	else{
		while(q->next && p->priority <= q->next->priority){q=q->next;}
		p->next = q->next;
		q->next = p;
	}
}

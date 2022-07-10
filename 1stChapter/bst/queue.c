#include<stdio.h>
#include<stdlib.h>
#include "bq.h"

int enqueue(QE **queue, NODE* node){
    QE* q = *queue;
    QE* r = (QE*)malloc(sizeof(QE));
    r->node = node;
    if(!q)
        *queue = r;
    else{
        while(q->next)
            q = q->next;
        q->next = r;
    }
    r->next = 0;
}

NODE* dequeue(QE **queue){
    QE* q = *queue;
    if(q){
        *queue=q->next;
        return q->node;
    }
    else
        return NULL;
}

int qlength(QE* queue){
    int len = 0;
    while(queue){
        ++len;
        queue = queue->next;
    }
    return len;
}

/*
int main(){
    QE* queue = NULL;
    NODE* node = (NODE*)malloc(sizeof(NODE));
    node->key = 10;
    node->next = 0;
    enqueue(&queue, node);
    printf("len is %d\n", qlength(queue));
    NODE* res = dequeue(&queue);
    printf("res is %d\n", res->key);
}
*/


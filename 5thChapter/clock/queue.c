#include<stdio.h>

int enqueue(PROC **queue, PROC *p){
    PROC *q = *queue;
    if(q==0 || p->priority > q->priority){
        *queue = p;
        p->next = q;
    }else{
        while(q->next && p->priority <= q->next->priority)
            q = q->next;
        p->next = q->next;
        q->next = p;
    }
}

PROC* dequeue(PROC **queue){
    PROC *q = *queue;
    if(q)
        *queue = q->next;
    return q;
}

int len(PROC *queue){
    int length = 0;
    PROC *t = queue;
    while(t){
        ++length;
        t = t->next;
    }
    return length;
}

int printList(char *name, PROC *p){
    printf("%s = ", name);
    while(p){
        printf("[%d %d]->", p->pid, p->priority);
        p = p->next;
    }
    printf("NULL\n");
}

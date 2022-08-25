#include<stdio.h>
#include<stdlib.h>
#include"type.h"
#include"queue.c"

PROC proc[NPROC];
PROC *freeList;
PROC *readyQueue;
PROC *sleepList;
PROC *running;

int tsleep(int event){}
int twakeup(int event){}
int texit(int status){}
int join(int pid, int *status){}

void func(void *param);
int init(){
    int i, j;
    PROC *p;
    for(i = 0;i < NPROC;i++){
        p = &proc[i];
        p->pid = i;
        p->priority = 0;
        p->status = FREE;
        p->event = 0;
        p->joinPid = 0;
        p->joinPtr = 0;
        p->next = p+1;
    }
    proc[NPROC-1].next = 0;
    freeList = &proc[0];        //all procs in freeList
    readyQueue = 0;
    sleepList = 0;
    //create p0 as initial running task
    running = p = dequeue(&freeList);
    p->status = READY;
    p->priority = 0;
    printList("freeList", freeList);
    printf("init complete:P0 running\n");
}


int texit(int value){
    printf("task %d in texit value=%d\n", running->pid, running->pid);
    running->status = FREE;
    running->priority = 0;
    enqueue(&freeList, running);
    printList("freeList:", freeList);
    tswitch();
}

int do_create(){
    int pid = create(func, running->pid);
}

int do_switch(){
    tswitch();
}

int do_exit(){
    texit(running->pid);
}

void func(void *param){
    int c;
    printf("task %d start:param=%d\n", running->pid, (int)param);
    while(1){
        printf("task %d running\n", running->pid);
        printList("readyQueue:", readyQueue);
        printf("enter a key [c|s|q]: ");
        c = getchar();getchar();
        switch(c){
            case 'c':do_create();break;
            case 's':do_switch();break;
            case 'q':do_exit();break;
            default:printf("invalid input\n");
        }
    }
}

int create(void (*f)(), void *parm){
    int i;
    PROC *p = dequeue(&freeList);
    if(!p){
        printf("create failed\n");
        return -1;
    }
    p->status = READY;
    p->priority = 1;
    p->joinPid = 0;
    p->joinPtr = 0;

    //initialize new task stack for it to resume to f(parm)
    for(i = 1; i < 13;++i)
        p->stack[SSIZE-i] = 0;
    p->stack[SSIZE-1] = (int)parm;          //function parameter
    p->stack[SSIZE-2] = (int)do_exit;       //return address
    p->stack[SSIZE-3] = (int)f;             //function entry
    p->ksp = (int)&p->stack[SSIZE-12];      //ksp->stack top
    enqueue(&readyQueue, p);
    printList("readyQueue", readyQueue);
    printf("task %d created a new task:%d\n", running->pid, p->pid);
    return p->pid;
}

int main(){
    printf("welcome to the MT user-level threads system\n");
    init();
    create((void*)func, 0);
    printf("P0 switch to P1\n");
    while(1){
        if(readyQueue)
            tswitch();
    }
}

int scheduler(){
    if(running->status == READY)
        enqueue(&readyQueue, running);
    running = dequeue(&readyQueue);
    printf("next running = %d\n", running->pid);
}

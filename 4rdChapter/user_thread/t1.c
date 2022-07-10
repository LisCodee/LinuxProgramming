#include<stdio.h>
#include<stdlib.h>
#include"type.h"
#include"queue.c"

PROC proc[NPROC];
PROC *freeList;
PROC *readyQueue;
PROC *sleepList;
PROC *running;

int tsleep(int event){
    running->event = event;
    running->status = SLEEP;
    enqueue(&sleepList, running);
    tswitch();
}
// wake up threads sleep on event, and return the num of wake up threads;
int twakeup(int event){
    PROC *p = dequeue(&sleepList);
    int l = len(sleepList), flag = 0;
    printf("there are %d threads in sleepList\n", l);
    while(l>0 && p){
        if(p->event == event){
            p->status = READY;
            enqueue(&readyQueue, p);
            printList("readyQueue", readyQueue);
            ++flag;
        }
        else
            enqueue(&sleepList, p);
        p = dequeue(&sleepList);
        --l;
    }
    return flag;
}
//exit the thread with status, if this thread have some threads join with it, wake up them then become a ZOMBIE;
int texit(int status){
    //try to find any task which wants to join with this task;
    //if no task wants to join with this task,exit as free
    running->status = ZOMBIE;
    running->exitStatus = status;
    int f = twakeup(running->pid);
    if(0 == f){
        running->status = FREE;
        enqueue(&freeList, running);
    }
    //give up cpu
    tswitch();
}
int join(int pid, int *status){
    printf("task %d try to join with %d:", running->pid, pid);
    int i, length=len(readyQueue);
    PROC *proc;
    for(i = 0;i < length;i++){
        proc = dequeue(&readyQueue); 
        if(proc->pid == pid){
           if(proc->joinPid == running->pid){
               printf("DEADLOCK ERROR!\n");
               return -1;
           }
           running->joinPid = pid;
           running->joinPtr = proc;
           if(proc->status == ZOMBIE){
               *status = proc->exitStatus;
               proc->status = FREE;
               enqueue(&freeList, proc);
               return proc->pid;
           }
           //sleep on target pid
           tsleep(pid);
           printList("sleepList:", sleepList);
        }
        enqueue(&readyQueue, proc);
    }
    printf("no thread id = %d\n", pid);
    return -1;
}
int create(void (*f)(), void *parm);
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

void task1(void *parm){     //task1:demonstrate create-join operations
    int pid[2];
    int i, status;
    for(i = 0; i< 2; ++i)
        pid[i] = create(func, running->pid);
    join(5, &status);       //try to join with targetPid=5
    join(2, &status);
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

int do_join(){
    int targetPid, status;
    printf("input a number that you want to join:");
    scanf("%d",&targetPid);
    int ret = join(targetPid, &status);
    printf("join exit with:%d\n", ret);
    return ret;
}

void func(void *param){
    int c;
    printf("task %d start:param=%d\n", running->pid, (int)param);
    while(1){
        printf("task %d running\n", running->pid);
        printList("readyQueue:", readyQueue);
        printList("sleepList:", sleepList);
        printList("freeList:", freeList);
        printf("enter a key [c|s|q|j]: ");
        c = getchar();getchar();
        switch(c){
            case 'c':do_create();break;
            case 's':do_switch();break;
            case 'q':do_exit();break;
            case 'j':do_join();break;
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
    int i,pid,status;
    printf("welcome to the MT user-level threads system\n");
    init();
    create((void*)task1, 0);
    printf("P0 switch to P1\n");
    tswitch();
    printf("all tasks ended:P0 loops\n");
    while(1){
    }
}

int scheduler(){
    if(running->status == READY)
        enqueue(&readyQueue, running);
    running = dequeue(&readyQueue);
}

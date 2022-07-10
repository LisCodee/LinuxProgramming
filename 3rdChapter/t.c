#include<stdio.h>
#include"type.h"
#include"queue.c"

int kfork();
int kexit();
int do_kfork();
int body();     //process body function
PROC proc[NPROC];
PROC *freeList;
PROC *readyQueue;
PROC *running;
/********************************************************
    kfrok() creates a child process; returns child pid.
    when scheduled to run, child PROC resumes to body();
********************************************************/
int kfork(){
    int i;
    PROC *p = dequeue(&freeList);
    if(!p){
        printf("no more proc\n");
        return -1;
    }/*initialize the new proc and its stack */

    /*initialize the new proc and its stack */
    p->status = READY;
    p->priority = 1;
    p->ppid = running->pid;
    for(i = 1; i < 10; i++)         //zero out kstack cells
        p->kstack[SSIZE-i] = 0;
    p->kstack[SSIZE-1] = (int)body; //retPC->body
    p->ksp = &(p->kstack[SSIZE-9]); //PROC.ksp -> saved eflag
    enqueue(&readyQueue, p);
    return p->pid;
}

int kexit(){
    running->status = FREE;
    running->priority = 0;
    enqueue(&freeList, running);
    printList("freeList", freeList);
    tswitch();
}

int do_kfork(){
    int child = kfork();
    if(child<0)
        printf("kfork failed\n");
    else{
        printf("proc %d kforked a child = %d\n", running->pid, child);
        printList("readyQueue", readyQueue);
    }
    return child;
}

int do_switch(){
    tswitch();
}

int do_exit(){
    kexit();
}

int body(){     //process body function
    int c;
    printf("proc %d starts from body()\n", running->pid);
    while(1){
        printf("*******************************\n");
        printf("proc %d running: parent=%d\n", running->pid, running->ppid);
        printf("enter a key [f|s|q]:\n");
        c = getchar();getchar();    //kill the \r key
        switch(c){
            case 'f':do_kfork();        break;
            case 's': do_switch();      break;
            case 'q': do_exit();        break;
        }
    }
}
//initialize the MT system; create P0 as initial running process
int init(){
    int i;
    PROC *p;
    for(i = 0; i < NPROC; i++){
        p = &proc[i];
        p->pid = i;
        p->status = FREE;
        p->priority = 0;
        p->next = p+1;
    }
    proc[NPROC-1].next = 0;
    freeList = &proc[0];
    readyQueue=0;
    //create P0 as the initial running process
    p = running = dequeue(&freeList);
    p->status = READY;
    p->ppid = 0;
    printList("freeList", freeList);
    printf("init complete: P0 running\n");
}

int main(){
    printf("Welcome to the MT Multitasking System\n");
    init();
    kfork();
    while(1){
        printf("P0:switch process\n");
        if(readyQueue)
            tswitch();
    }
}

//scheduler
int scheduler(){
    printf("proc %d in scheduler()\n", running->pid);
    if(running->status == READY)
        enqueue(&readyQueue, running);
    printList("readyQueue", readyQueue);
    running = dequeue(&readyQueue);
    printf("next running= %d\n", running->pid);
}




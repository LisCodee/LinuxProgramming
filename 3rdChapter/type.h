#define NPROC 9
#define SSIZE 1024

//PROC status
#define FREE 0
#define READY 1
#define SLEEP 2
#define ZOMBIE 3

typedef struct proc{
    struct proc *next;  //net proc pointer
    int *ksp;           //saved stack pointer
    int pid;
    int ppid;
    int status;
    int priority;
    int kstack[SSIZE];
    int event;          //event value to sleep on
    int exitCode;       //exit value
    struct proc *child; //first child PROC pointer
    struct proc *sibling;//sibling PROC pointer
    struct proc *parent;//parent PROC pointer
}PROC;

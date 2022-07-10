#define NPROC   9
#define SSIZE   1024
#define FREE    0
#define READY   1
#define SLEEP   2
#define BLOCK   3
#define ZOMBIE  4

typedef struct proc{
    struct proc* next;
    int ksp;                //saved stack pointer
    int pid;                //proc PID
    int priority;           //proc schedule priority
    int status;             //current status
    int event;              //for sleep/wakeup
    int exitStatus;         //exit status
    int joinPid;            //join targe pid
    struct proc* joinPtr;   //join target ptr
    int stack[SSIZE];       //proc 4KB stack area
}PROC;



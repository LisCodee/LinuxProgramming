#include <stdio.h>

int main()
{
    printf("THIS IS %d, PARENT IS %d\n", getpid(), getppid());
    int pid = fork();

    if (pid)
    {
        nice(2);
        sched_yield();
        printf("this is process %d, child is %d\n", getpid(), pid);
        sleep(1);
    }
    else
    {
        sleep(3);
        printf("this is process %d, parent is %d\n", getpid(), getppid());
    }

    return 0;
}

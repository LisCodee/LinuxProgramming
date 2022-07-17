#include<stdio.h>
#include<signal.h>
#include<string.h>

#define LEN 64

int ppipe[2];
int pid;
char line[LEN];

int parent(){
    printf("parent %d running\n", getpid());
    close(ppipe[0]);
    while(1){
        printf("parent %d: input a line: \n", getpid());
        fgets(line, LEN, stdin);
        line[strlen(line)-1] = 0;
        printf("parent %d write to pipe\n", getpid());
        write(ppipe[1], line, LEN);
        printf("parent %d send singal 10 to %d\n", getpid(), pid);
        kill(pid, SIGUSR1);
    }
}

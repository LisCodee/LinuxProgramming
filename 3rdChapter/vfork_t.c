#include<stdio.h>

int main(){
    printf("THIS IS %d, PARENT IS %d\n", getpid(), getppid());
    int pid = vfork();

    if(pid){
        printf("this is parent process %d, child is %d\n", getpid(), pid);
    }
    else{
        printf("this is child process %d, parent is %d\n", getpid(), getppid());
        int i = 0;
        for(;i < 10; i++)
            printf("i = %d\t", i);
        printf("\n");
    }
}

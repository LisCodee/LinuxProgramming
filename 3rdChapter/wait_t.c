#include<stdio.h>
#include<stdlib.h>

int main(){
    int pid, status;
    pid = fork();
    if(pid){
        printf("PARENT %d WAITS FOR CHILD %d TO DEAD\n", getpid(), pid);
        pid = wait(&status);
        printf("DEAD CHILD=%d, status=0x%04x\n", pid, status);
    }else{
        printf("child %d dies by exit(VALUE)\n", getpid());
        exit(100);
    }
    return 0;
}

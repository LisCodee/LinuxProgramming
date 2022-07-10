#include<stdio.h>
#include<unistd.h>
#include<wait.h>
#include<sys/prctl.h>

int main(){
    int pid, r, status;
    printf("mark process %d as a subreaper\n", getpid());
    r = prctl(PR_SET_CHILD_SUBREAPER);
    pid = fork();
    if(pid){
        printf("subreaper pid=%d child pid=%d\n", getpid(), pid);
        while(1){
            pid = wait(&status);
            if(pid > 0)
                printf("subreaper %d waited a ZOMBIE=%d\n", getpid(), pid);
            else
                break;
        }
    }else{
        printf("child=%d parent=%d\n", getpid(), getppid());
        pid = fork();
        if(pid){
            printf("child=%d start: grandchild=%d\n", getpid(), pid);
            printf("child=%d EXIT: grandchild=%d\n", getpid(), pid);
        }
        else{
            sleep(1);
            printf("grandchild=%d start: myparent=%d\n", getpid(), getppid());
            printf("grandchild=%d EXIT: myparent=%d\n", getpid(), getppid());
        }
    }
}

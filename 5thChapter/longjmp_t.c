#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<setjmp.h>

jmp_buf env;
int count = 0;

void handler(int sig, siginfo_t *siginfo, void *context){
    printf("handler:sig=%d from pid=%d uid=%d count=%d\n",sig, siginfo->si_pid, siginfo->si_uid, ++count);
    if(count >= 4)
        longjmp(env, 1234);
}

int BAD(){
    int *ip = 0;
    printf("in BAD(): try to dereference NULL pointer\n");
    *ip = 10;
    printf("this line should't appear\n");
}

int main(int argc, char *argv[]){
    int r;
    struct sigaction act;
    memset(&act, 0, sizeof(act));
    act.sa_sigaction = &handler;
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGSEGV, &act, NULL);
    if((r=setjmp(env))==0)
        BAD();
    else
        printf("proc %d survived SETGMENT ACTION FAULT:r=%d\n", getpid(), r);
    printf("proc %d looping\n", getpid());
    while(1);
}

#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<sys/time.h>

int count = 0;
struct itimerval t;

void timer_handler(int sig){
    printf("timer_handler: signal=%d count=%d\n", sig, ++count);
    if(count >= 8){
        printf("cancel timer\n");
        t.it_value.tv_sec = 0;
        t.it_value.tv_usec = 0;
        setitimer(ITIMER_VIRTUAL, &t, NULL);
    }
}

int main(){
    struct itimerval timer;
    //install timer_handler as SIGVTALRM signal handler
    signal(SIGVTALRM, timer_handler);
    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = 100000;
    timer.it_interval.tv_sec = 1;
    timer.it_interval.tv_usec = 0;
    setitimer(ITIMER_VIRTUAL, &timer, NULL);
    printf("looping: enter control-c to terminal\n");
    while(1);
}

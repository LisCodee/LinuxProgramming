#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>
#include<time.h>


int main(){
    struct timeval t;
    gettimeofday(&t, NULL);
    printf("sec = %ld, usec=%d\n", t.tv_sec, t.tv_usec);
    printf((char*)ctime(&t.tv_sec));
}

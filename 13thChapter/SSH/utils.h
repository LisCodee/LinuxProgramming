#include<stdio.h>
#include<errno.h>

void printError()
{
    printf("errno:[%d], error:[%s]\n", errno, strerror(errno));
    exit(-1);
}



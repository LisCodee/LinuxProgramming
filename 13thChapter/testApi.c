#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>


int main(int argc, char* argv[])
{
    char host[1024];
    int i = gethostname(host, 1024);
    if(i == 0)
        printf("host name is %s\n", host);

    return 0;
}


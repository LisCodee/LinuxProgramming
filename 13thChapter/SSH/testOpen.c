#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<errno.h>




int main(int argc, char* argv[])
{

    char name[] = "a.txt";
    int fd = open(name, O_CREAT|O_RDWR, 0666);
    write(fd, name, sizeof(name));
    printf("errno%d, error%s\n", errno, strerror(errno));
    close(fd);

    return 0;
}


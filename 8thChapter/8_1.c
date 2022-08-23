#include<stdio.h>
#include<errno.h>

int main(int argc, char* argv[])
{
    if(argc <= 1)
    {
        printf("INPUT ERROR\n");
        return -1;
    }
    printf("you will make %d dirs\n", argc-1);
    int i = 0;
    int out = 0;
    for(;i < argc;++i)
    {
        printf("making dir:argv[%d]:%s\n", i, argv[i]);
        if(i > 0)
        {
            out = mkdir(argv[i], 0766);
            if(out < 0){
                printf("errno=%d : %s\n", errno, strerror(errno));
                continue;
            }
            else
                printf("making dir [%s] success!\n", argv[i]);
        }
    }
    return 0;
}

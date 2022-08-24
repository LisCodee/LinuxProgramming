#include<stdio.h>

int main()
{
    //setvbuf(stdout, NULL, _IONBF, 0);
    while(1)
    {
        printf("hi ");
        fflush(stdout);
        sleep(1);
    }
    return 0;
}

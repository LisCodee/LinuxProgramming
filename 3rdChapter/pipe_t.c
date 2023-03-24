#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int pd[2], n, i;
char line[256];

int main()
{
    pipe(pd);
    printf("pd=[%d:%d]\n", pd[0], pd[1]);
    if(fork())
    {
        printf("parent %d close pd[0]\n", getpid());
        close(pd[0]);
        while(1)
        {
            printf("parent %d writeing to pipe\n", getpid());
            n = write(pd[1], "I am your papa", 16);
            printf("parent wrote %d bytes to pipe\n", n);
        }
        printf("parent %d exit.\n", getpid());
        sleep(2);
    }
    else
    {
        printf("child %d close pd[1]", getpid());
        close(pd[1]);
        while(1)
        {
            printf("child reading from pipe...\n");
            if((n = read(pd[0], line, 128)) && ++i < 5)
            {
                line[n] = 0;
                printf("chile recv %d bytes from pipe:%s\n", n, line);
            }
        }
    }

}
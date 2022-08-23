#include<stdio.h>
#include<dirent.h>

int main()
{
    struct dirent *ep;
    DIR* dp = opendir("aaa");
    while(ep = readdir(dp))
    {
        printf("name=%s ", ep->d_name);
    }
    return 0;
}

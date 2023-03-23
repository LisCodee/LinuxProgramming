#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<netdb.h>

char myname[64];
struct sockaddr_in server_addr, sock_addr;


int main(int argc, char* argv[])
{
    gethostname(myname, 64);
    struct hostent *hp = gethostbyname(myname);
    if(hp == 0)
    {
        printf("unknow host %s\n", myname);
        exit(1);
    }
    printf("hp->h_name:%s\n", hp->h_name);
    printf("hp->h_addrtype:%d\n", hp->h_addrtype);
    printf("hp->h_length:%d\n", hp->h_length);
    int i = 0;
    printf("hp->h_addr:%s\n", inet_ntoa(*(long*)(hp->h_addr_list[i])));
    return 0;
}


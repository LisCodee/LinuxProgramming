#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<netdb.h>

#define MAX 256
#define SERVER_HOST "localhost"
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 1234

struct sockaddr_in server_addr, client_addr;

int mysocket, csocket;
int r,len,n;

int server_init()
{
    printf("=================server init==================\n");
    printf("1. create a TCP socket\n");
    mysocket = socket(AF_INET, SOCK_STREAM, 0);
    if(mysocket < 0)
    {
        printf("socket call failed\n");
        exit(1);
    }

    printf("2. fill server_addr with host IP and PORT\n");
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(SERVER_PORT);

    printf("3. bind socket to server address\n");
    r = bind(mysocket, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if(r < 0)
    {
        printf("bind failed\n");
        exit(2);
    }
    printf("hostname=%s ,port=%d\n", SERVER_HOST, SERVER_PORT);
    printf("4. server is listening...\n");
    listen(mysocket, 5);    //queue length=5
    printf("====================init done=================\n");
}

int main()
{
    char line[MAX];
    server_init();
    while(1)
    {
        printf("server: accepting a new connection\n");
        len = sizeof(client_addr);
        csocket = accept(mysocket, (struct sockaddr*)&client_addr, &len);
        if(csocket < 0){
            printf("server accept error\n");
            exit(3);
        }
        printf("server: accepted a connection from client\n");
        printf("------------------------------------------\n");
        printf("clinet:IP=%s, port=%d\n", inet_ntoa(client_addr.sin_addr.s_addr), ntohs(client_addr.sin_port));
        printf("-------------------------------------------\n");
        while(1)
        {
            n = read(csocket, line, MAX);
            if(n == 0)
            {
                printf("server: client died, server loops\n");
                close(csocket);
                break;
            }
            printf("server:wrote n=%d bytes; ECHO=%s\n", n, line);
            n = write(csocket, line, MAX);
            printf("server: wrote %d bytes; ECHO=%s\n", n, line);
            printf("server: ready for next request\n");
        }
    }
}



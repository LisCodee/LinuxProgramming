#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<netdb.h>

#define MAX 256
#define SERVER_HOST "localhost"
#define SERVER_PORT 1234

struct sockaddr_in server_addr;
int sock, r;

int clientInit()
{
    printf("=========client init===========\n");
    printf("1. create a tcp socket\n");
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
    {
        printf("call socket failed\n");
        exit(1);
    }

    printf("2. fill server_addr with server ip and host\n");
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(SERVER_PORT);

    printf("3. connecting to server...\n");
    r = connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if(r < 0)
    {
        printf("connect failed\n");
        exit(2);
    }

    printf("4. connect OK to\n");
    printf("--------------------------------------\n");
    printf("Server hostname=%s PORT=%d\n", SERVER_HOST, SERVER_PORT);
    printf("--------------------------------------\n");
    printf("=============init done===============\n");
}


int main(int argc, char* argv[])
{
    int n;
    char line[MAX], ans[MAX];
    clientInit();
    printf("processing loop\n");
    while(1)
    {
        printf("input a line:");
        bzero(line, MAX);
        fgets(line, MAX, stdin);
        line[strlen(line)-1] = 0;
        if(line[0] == 0)
            exit(0);
        n = write(sock, line, MAX);
        printf("client: wrote %d bytes, line=%s\n", n, line);
        n = read(sock, ans, MAX);
        printf("client: read %d bytes, echo=%s\n", n, ans);
    }
    return 0;
}


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<netdb.h>
#include<sys/socket.h>

#include "utils.h"

#define LEN 1024
#define SERVER_HOST "localhost"
#define SERVER_PORT 1209

void printError();

struct sockaddr_in sAddr;
int mySock;

void initClient()
{
    printf("===================init Client...===================\n");
    mySock = socket(AF_INET, SOCK_STREAM, 0);
    if(mySock == -1)
        printError();
    memset(&sAddr, 0, sizeof(sAddr));

    sAddr.sin_family = AF_INET;
    sAddr.sin_port = htons(SERVER_PORT);
    inet_aton(SERVER_HOST, &sAddr.sin_addr.s_addr);
    
    if(connect(mySock, (struct sockaddr*)&sAddr, sizeof(sAddr)) == -1)
        printError();
    printf("==================init done=========================\n");
    printf("connect to host:[%s:%d]\n", SERVER_HOST, SERVER_PORT);
}

int main(int argc, char* argv[])
{
    char cmd[LEN], res[LEN];
    initClient();
    while(1)
    {
        printf("input the cmd and param(sep with space):");
        fgets(cmd, LEN, stdin);
        cmd[strlen(cmd)-1] = '\0';
        //printf("your command is : %s\n", cmd);
        int n;
        n = write(mySock, cmd, LEN);
        n = read(mySock, res, LEN);
        if(n <= 0)
            continue;
        printf("get %d byte from server:%s\n", LEN, res);
    }
    return 0;
}


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netdb.h>
#include<errno.h>
#include<sys/stat.h>

#include "utils.h"

#define LEN 1024
#define SERVER_HOST "127.0.0.1"
#define SERVER_PORT 1209
#define LISTEN_BACKLOG 512

void printError();
void initServer();

struct sockaddr_in myAddr, cAddr;
int mySock, cSock;

void initServer()
{
    printf("=================init ssh Server...=================\n");
    mySock = socket(AF_INET, SOCK_STREAM, 0);
    if(mySock == -1)
        printError();
    memset(&myAddr, 0, sizeof(myAddr));
    myAddr.sin_family = AF_INET;
    inet_aton(SERVER_HOST, &myAddr.sin_addr.s_addr);
    myAddr.sin_port = htons(SERVER_PORT);

    if(bind(mySock, (struct sockaddr*)&myAddr, sizeof(myAddr)) == -1)
        printError();

    if(listen(mySock, LISTEN_BACKLOG) == -1)
        printError();

    printf("===================init done========================\n");
}


int execute(char* command, char* res, int len)
{
    char cmd[16], param[256];
    char* token;
    token = strtok(command, " ");
    strcpy(cmd, token);
    printf("command is [%s]\n", cmd);
    printf("%d\n", strcmp(cmd, "pwd"));
    if(!strcmp(cmd, "pwd"))
    {
        printf("command is [%s]\n", cmd);
        getcwd(res, len);
        return 0;
    }
    if(!strcmp(cmd, "mkdir"))
    {
        printf("command is [%s]\n", cmd);
        token = strtok(NULL, " ");
        if(token == NULL)
            return -1;
        strcpy(param, token);
        printf("mkdir's param is %s\n", param);
        mkdir(param, 0755);
        strcpy(res, "mkdir success\n");
        return 0;
    }
    return -1;
}

int main(int argc, char* argv[])
{
    char cmd[LEN];
    initServer();
    while(1)
    {
       printf("=================ssh server listening...===================\n");
       int len = sizeof(cAddr);
       cSock = accept(mySock, (struct sockaddr*)&cAddr, &len);
       if(cSock == -1)
           printError();
       printf("connect to client:%s, port:%d\n", inet_ntoa(cAddr.sin_addr.s_addr), ntohs(cAddr.sin_port));
       while(1)
       {
           int s = 0;
           char output[LEN];
           s = read(cSock, cmd, LEN);
           if(s == -1)
               printError();
           else if(s == 0)
           {
               printf("read zero byte from client socket\n");
               close(cSock);
               break;
           }else
           {
               if(execute(cmd, output, LEN) == 0)
               {
                   printf("execute result is: %s\n", output);
                   //write res to client
                   write(cSock, output, LEN);
               }else
               {
                   printf(" cmd error\n");
                   strcpy(output, "command error, try agagin\n");
                   write(cSock, output, LEN);
               }
               printf("wating for next request\n");
           }
       }
    }
    return 0;
}


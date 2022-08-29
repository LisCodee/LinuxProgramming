#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netdb.h>
#include<errno.h>
#include<sys/stat.h>
#include<dirent.h>
#include<fcntl.h>

#include "utils.h"

#define LEN 1024
#define SERVER_HOST "127.0.0.1"
#define SERVER_PORT 1209
#define LISTEN_BACKLOG 512

void printError();
int parseCmd(const char* command, char* c, char* p);
int parsePath(const char* path, char* prefix, char* basename);

struct sockaddr_in myAddr, cAddr;
int mySock, cSock;
char root[] = "/home/xpt/Linux/13thChapter/SSH";
char cwd[LEN] = "/home/xpt/Linux/13thChapter/SSH";

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

int listDir(char* path, char* output)
{    
    struct dirent* entry;
    if(path == NULL || strlen(path) == 0)
    {
        DIR* dirp = opendir(cwd);
        while(entry = readdir(dirp))
        {
            strcat(output, "\n");
            strcat(output, entry->d_name);
            strcat(output, "\t");
        }
        return 0;
    }
    char prefix[LEN], basename[LEN], destPath[2*LEN];
    if(parsePath(path, prefix, basename) == 1)
    {
        strcpy(destPath, cwd);
    }else{
        if(strstr(prefix, root) == NULL)
            return -1;
        strcpy(destPath, prefix);
    }
    strcat(destPath, "/");
    strcat(destPath, basename);
    struct stat s;
    if(stat(destPath, &s) == -1)
    {
        return -2;
    }
    if(S_ISDIR(s.st_mode))
    {
        DIR* dirp = opendir(destPath);
        while(entry = readdir(dirp))
        {
            strcat(output, "\n");
            strcat(output, entry->d_name);
            strcat(output, "\t");
        }
        return 0;
    }
    else
        return -3;
}

int makeDir(char* path)
{
    char prefix[LEN], basename[LEN], destPath[2*LEN];
    if(parsePath(path, prefix, basename) == 1)
    {
        strcpy(destPath, cwd);
    }else{
        if(strstr(prefix, root) == NULL)
            return -1;
        strcpy(destPath, prefix);
    }
    strcat(destPath, "/");
    strcat(destPath, basename);
    printf("prefix is [%s], basename is [%s]\n", prefix, basename);
    if(mkdir(destPath, 0755) == -1)
        return -2;
    return 0;
}

int rm(char* path)
{
    char prefix[LEN], basename[LEN], destPath[2*LEN];
    if(parsePath(path, prefix, basename) == 1)
        strcpy(destPath, cwd);
    else
    {
        if(strstr(prefix, root) == NULL)
            return -1;
        strcpy(destPath, prefix);
    }
    strcat(destPath, "/");
    strcat(destPath, basename);
    struct stat s;
    if(stat(destPath, &s) == -1)
    {
        return -2;
    }
    if(S_ISDIR(s.st_mode))
        if(rmdir(destPath) == -1)
            return -2;
    if(S_ISREG(s.st_mode))
        if(unlink(destPath) == -1)
            return -2;
    return 0;
}

int changeDir(char* path)
{
    char prefix[LEN], basename[LEN], destPath[2*LEN];
    printf("DEBUG:path is:%s\n", path);
    if(!strcmp(path, "."))
        return 0;
    if(!strcmp(path, ".."))
    {
        if(!strcmp(root, cwd))  //if cwd == root, permission deined
            return -1;
        else
        {
            char* idx = rindex(cwd, (int) '/');
            *idx = '\0';
            return 0;
        }
    }
    if(parsePath(path, prefix, basename) == 1)
        strcpy(destPath, cwd);
    else
    {
        if(strstr(prefix, root) == NULL)
            return -1;  //permission deined
        strcpy(destPath, prefix);
    }
    strcat(destPath, "/");
    strcat(destPath, basename);
    printf("DEBUG:prefix:%s,basename:%s, destPath:%s\n", prefix, basename, destPath);
    struct stat s;
    if(stat(destPath, &s) == -1)
        return -2;      //syscall error
    if(S_ISDIR(s.st_mode))
    {
        strcpy(cwd, destPath);
        return 0;
    }
    else
        return -3;      //no such dir
}

int getFile(char* path, int cSock)
{
    printf("DEBUG:path->%s\n", path);
    char prefix[LEN], basename[LEN], destPath[2*LEN];
    if(parsePath(path, prefix, basename) == 1)
        strcpy(destPath, cwd);
    else
    {
        if(strstr(prefix, root) == NULL)
            return -1;  //permission deined
        strcpy(destPath, prefix);
    }
    strcat(destPath, "/");
    strcat(destPath, basename);
    struct stat s;
    if(stat(destPath, &s) == -1)
        return -2;
    if(S_ISREG(s.st_mode))  //if is regular file, write to cSock
    {
        int size, fd;
        char temp[LEN];
        fd = open(destPath, O_RDONLY);
        //write(cSock, temp, 0);      //first zero byte to start transport
        while(size = read(fd, temp, LEN))
        {
            if(size == -1)
                return -2;  //sys error
            else if(size == 0)
            {
                write(cSock, temp, size);   //zero byte to end transport
                return 0;   //read/write done
            }
            else
                write(cSock, temp, size);
        }
    }
    else
        return -3;      //no such file
}

int execute(const char* command, char* res, int len, int cSock)
{
    char cmd[16], param[LEN];
    memset(param, 0, LEN);
    parseCmd(command, cmd, param);
    if(!strcmp(cmd, "pwd"))
    {
        strcpy(res, cwd);
        return 0;
    }
    if(!strcmp(cmd, "mkdir"))
    {
        int r = makeDir(param);
        if(r == -1)
            strcpy(res, "Permission deined!\n");
        if(r == -2)
            strcpy(res, strerror(errno));
        if(r == 0)
            strcpy(res, "success\n");
        return 0;
            
    }
    if(!strcmp(cmd, "rm"))
    {
        int r = rm(param);
        if(r == -1)
            strcpy(res, "permission deined!\n");
        if(r == -2)
            strcpy(res, strerror(errno));
        if(r == 0)
            strcpy(res, "success\n");
        return 0;
    }
    if(!strcmp(cmd, "cd"))
    {
       int r = changeDir(param);
       if(r == -1)
            strcpy(res, "permission deined!\n");
       if(r == -2)
            strcpy(res, strerror(errno));
       if(r == -3)
            strcpy(res, "No such dir\n"); 
       if(r == 0)
            strcpy(res, "success\n");
       printf("cwd:%s\n", cwd);
       return 0;
    }
    if(!strcmp(cmd, "ls"))
    {
       int r = listDir(param, res);
       if(r == -1)
            strcpy(res, "permission deined!\n");
       if(r == -2)
            strcpy(res, strerror(errno));
       if(r == -3)
            strcpy(res, "No such dir\n"); 
       printf("cwd:%s\n", cwd);
       return 0;
    }
    if(!strcmp(cmd, "get"))
    {
       int r = getFile(param, cSock);
       if(r == -1)
            strcpy(res, "permission deined!\n");
       if(r == -2)
            strcpy(res, strerror(errno));
       if(r == -3)
            strcpy(res, "No such File\n"); 
       if(r == 0)
            strcpy(res, "");
       return 0; 
    }
    if(!strcmp(cmd, "put"))
    {
        //putFile(param);
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
               if(execute(cmd, output, LEN, cSock) == 0)
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


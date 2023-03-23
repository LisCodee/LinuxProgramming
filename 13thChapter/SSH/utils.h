#include<stdio.h>
#include<errno.h>
#include<string.h>

void printError()
{
    printf("errno:[%d], error:[%s]\n", errno, strerror(errno));
    exit(-1);
}

int parseCmd(const char* command, char* c, char* p)
{
    char* cmd = (char*)malloc(sizeof(command));
    strcpy(cmd, command);
    char* token = strtok(cmd, " ");
    if(token == NULL)
        strcpy(c, command);
    strcpy(c, token);
    token = strtok(NULL, " ");
    if(token != NULL)
        strcpy(p, token);
    else
        p = NULL;
    printf("DEBUG:cmd:%s,param:%s\n", c, p);
    return 0;
}

int parsePath(const char* p, char* prefix, char* basename)
{
    char path[1024];
    strcpy(path, p);
    char* token = strrchr(path, (int) '/');
    if(token == NULL)
    {
        strcpy(prefix, "");
        strcpy(basename, path);
        return 1;
    }
    strcpy(basename, token+1);
    *token = '\0';
    strcpy(prefix, path);
    return 0;
}



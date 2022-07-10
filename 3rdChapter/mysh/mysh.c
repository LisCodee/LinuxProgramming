#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>

#define LEN_PATH 128
//get the env list;
void matchPath(char* env[], char* path[], int len_pth){
    int i = 0;
    while(1){
        char* p = "PATH";
        if(!memcmp(p, env[i], strlen(p)))
            break;
        ++i;
    }
    char *token = strtok(env[i], "=");
    if(token){
        printf("%s:\n", token);
        i = 0;
        while(token){
            token = strtok(NULL, ":");
            if(!token)
                break;
            path[i++] = token;
        }
        i = 0;
        while(path[i]){
            printf("%s\n", path[i++]);
        }

    }else{
        printf("There is no path in env\n");
        exit(0);
    }
}
//get input and process to cmd and param;
void getinput(char* const env[]){
    while(1){
        printf("mysh \%:");
        fflush(stdout);
        char *cmd, *param[LEN_PATH], input[1024], fullpath[64] = "/bin/";
        int i = 0;
        for(;i < LEN_PATH; i++)
            param[i] = NULL;
        int fd = 0;
        //scanf("%s", input);
        read(0,input,1024);
        if(strlen(input)<=1)
            continue;
        char *find = strchr(input, '\n');
        *find = '\0';
        char *token;
        token = strtok(input, " ");
        cmd = token;
        i = 1;
        while(token){
            token = strtok(NULL, " ");
            param[i++] = token;
        }
        i = 0;
        param[i] = cmd;
        printf("params:\t");
        strcat(fullpath, cmd);
        while(param[i])
            printf("%s\t", param[i++]);
        printf("\n");
        int pid = vfork();
        if(!pid){
            printf("cmd is:%s\n", fullpath);
            printf("child process:%d\n", getpid());
            execve(fullpath, param, env);
            return;
        }else{
            //让子进程先执行
            nice(5);
            sched_yield();
            int status = 0;
            pid = wait(&status);
            printf("%d process dead with status %d\n", pid, status);
        }
    }
}

//main process
int main(int argc, char* argv[], char* env[]){
    printf("*********************mysh*********************\n");
    char* path[LEN_PATH];
    int i = 0;
    for(;i<128;i++)
        path[i] = NULL;
    matchPath(env, path, LEN_PATH);
    getinput(env);
}

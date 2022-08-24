#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
#include<time.h>
#include<sys/types.h>
#include<dirent.h>

struct stat mystat, *sp;
char* t1 = "xwrxwrxwr-------";
char* t2 = "----------------";

char* readLink(char* fname)
{
    char str[] = "nothing\n";
    return str;
}

int lsFile(char* fname)
{
    struct stat fstat, *sp;
    int r, i;
    char ftime[64];
    sp = &fstat;
    if( (r = lstat(fname, &fstat)) < 0)
    {
        printf("can't stat %s\n", fname);
        exit(1);
    }
    if( (sp->st_mode & 0xF000) == 0x8000)
        printf("%c", '-');
    if( (sp->st_mode & 0xF000) == 0x4000)
        printf("%c", 'd');
    if( (sp->st_mode & 0xF000) == 0xA000)
        printf("%c", 'l');
    for( i = 8; i >= 0;--i)
    {
        if(sp->st_mode & (1 << i))
            printf("%c", t1[i]);
        else
            printf("%c", t2[i]);
    }
    printf("%4d ", sp->st_nlink);
    printf("%4d ", sp->st_gid);
    printf("%4d ", sp->st_uid);
    printf("%8d ", sp->st_size);
    //print time
    strcpy(ftime, ctime(&sp->st_ctime));
    ftime[strlen(ftime)-1] = 0;
    printf("%s ", ftime);
    //print name
    printf("%s ", basename(fname));
    //print linkname if symbolic file
    if((sp->st_mode & 0xF000) == 0xA000)
        printf(" -> %s", readLink(fname));
    printf("\n");
}

int lsDir(char *dname)
{
    struct dirent *ep;
    DIR* dp = opendir(dname);
    while(ep = readdir(dp))
    {
        printf("%s\n", ep->d_name);
    }
    return 0;
}


int main(int argc, char* argv[])
{
    struct stat mystat, *sp = &mystat;
    int r;
    char* filename, path[1024], cwd[256];
    filename = "./";
    if(argc > 1)
        filename = argv[1];
    if(r = lstat(filename, sp) < 0)
    {
        printf("no such file %s\n", filename);
        exit(1);
    }
    strcpy(path, filename);
    if(path[0] != '/')
    {
        getcwd(cwd, 256);
        strcpy(path, cwd);
        strcat(path, "/");
        strcat(path, filename);
    }
    if(S_ISDIR(sp->st_mode))
        lsDir(path);
    else
        lsFile(path);
    return 0;
}


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "type.h"
#define LEN 128

int getBasename(const char* path, char* dirname, char* basename);
void initialize();
NODE* findNodeFromParent(NODE* parent, char* name);
NODE* findAbsNode(const char* dirname, char* msg);
NODE* findRelNode(const char *dirname, char* msg);
int isDir(NODE *n);
int isLeaves(NODE *n);
int mkdir(char* name);
int findCmd(char* c);
NODE* findLastChild(NODE* parent);
int rmdir(const char* dir);
int cd(const char* dir);
int ls(const char* dir);
int pwd(const char* dir);
int create(const char* path);
int rm(const char* path);
int reload(const char* path);
int menu(const char* path);
int quit(const char* path);
int save(NODE* root, char* predir);


NODE *root, *cwd;                   //root and cwd pointers
char line[LEN];                     //user input
char command[16],pathname[64];      //command and pathname strings
char dname[64],bname[64];           //dir name and base name
char *cmd[] = {"mkdir", "rmdir", "ls", "cd", "pwd", "create", "rm", "reload", "save", "menu", "quit", NULL};
//function pointer to the system functions
int (*funPtr[]) (char*) = {(int (*)()) mkdir, rmdir, ls, cd, pwd, create, rm, reload, save, menu, quit};
FILE *fp;

//get dirname and basename from dirname
int getBasename(const char* path, char* dirname, char* basename){
    int str_len = strlen(path);
    int last_index = str_len-1;
    for(;path[last_index]!='/' && last_index > 0;--last_index){}
//    printf("last_index is %d, char is %c\n", last_index, path[last_index]);
    int start = 0;
    for(;start<last_index;++start){
        dirname[start] = path[start];
    }
    dirname[start] = '\0';
//    printf("start is %d, char is %c\n", start, path[start]);
    int base_start = 0;
    for(start=last_index+1;start <= str_len;start++)
        basename[base_start++] = path[start];
    return 0;
}

NODE* findNodeFromParent(NODE* parent, char* name){
    NODE* p = parent->childPtr;
    while(p){
        if(!strcmp(p->name, name))
            return p;
        p = p->siblingPtr;
    }
    return NULL;
}

NODE* findAbsNode(const char* dirname, char* msg){
    int str_len = strlen(dirname);
    NODE* res = root->childPtr;
    char name[64];
    int name_count=0, path_count = 1;
    while(path_count < str_len){
        if(dirname[path_count] == '/' || dirname[path_count] == '\0'){
            name[name_count] = '\0';
            res = findNodeFromParent(res, name);
            if(!res){
                msg = "No such file or dictionary";
                return NULL;
            }
            name_count = 0;
        }
        name[name_count++] = dirname[path_count++]; 
    }
    return res;
}

NODE* findRelNode(const char *dirname, char* msg){
    int str_len = strlen(dirname);
    NODE* res = cwd->childPtr;
    char name[64];
    int name_count=0, path_count = 0;
    while(path_count < str_len){
        if(dirname[path_count] == '/' || dirname[path_count] == '\0'){
            name[name_count] = '\0';
            res = findNodeFromParent(res, name);
            if(!res){
                msg = "No such file or dictionary";
                return NULL;
            }
            name_count = 0;
        }
        name[name_count++] = dirname[path_count++]; 
    }
    return res;

}

int isDir(NODE *n){
    return n->node_t == 'D';
}

int isLeaves(NODE *n){
    return n->childPtr == NULL;
}

int save(NODE* root, char* predir){
    if(!root)   return 0;
    char path[64];
    if(strcmp(root->name, "/"))
        strcpy(path, strcat(strcat(predir,"/"),root->name));
    else
        strcpy(path, root->name);
        //path = root->name;
    fprintf(fp, "%c %s", root->node_t, path);
    fprintf(fp, "\n");
    save(root->childPtr, path);
    save(root->siblingPtr, predir);
}

NODE* findLastChild(NODE* parent){
    NODE* temp = parent;
    if(!temp->childPtr)   return parent;
    temp = temp->childPtr;
    while(temp->siblingPtr){
        temp = temp->siblingPtr;
    }
    return temp;
}

int mkdir(char* name){
    char dirname[64], basename[64];
    getBasename(name, dirname, basename);
    NODE* parent;
    char msg[128];
    if(name[0] == '/')
        parent = findAbsNode(dirname, msg);
    else 
        parent = findRelNode(dirname, msg);
    if(strlen(msg)){
        printf("%s", msg);
        return -1;
    }
    if(parent->node_t == 'D'){
        NODE* temp = findNodeFromParent(parent, basename);
        if(temp){
            return -1;
        }else{
            NODE* n = (NODE*)malloc(sizeof(NODE));
            n->node_t = 'D';
            //n->name = basename;
            strcpy(n->name, basename);
            n->parentPtr = parent;
            n->childPtr = NULL;
            n->siblingPtr = NULL;
            NODE* t = findLastChild(parent);
            t->siblingPtr = n;
        }
    }else
        return -1;
}

int rmdir(const char* dir){
}

int cd(const char* dir){
}

int ls(const char* dir)
{}
int pwd(const char* dir)
{}
int create(const char* path)
{}
int rm(const char* path)
{}
int reload(const char* path)
{}
int menu(const char* path)
{}
int quit(const char* path)
{}
//find input in cmd
int findCmd(char* c){
    int i = 0;
    while(cmd[i]){
        if(strcmp(c, cmd[i]))
            return i;
        ++i;
    }
    return -1;
}

void initialize(){
    fp = fopen("myfile.txt", "w+");
    root = (NODE*)malloc(sizeof(NODE));
    //root->name = name;  assigment to expression with array type
    strcpy(root->name, "/");
    root->node_t = 'D';
    root->childPtr = NULL;
    root->siblingPtr = NULL;
}

int main(){
    initialize();//initialize root node of the file sys tree
    char dirname[64], basename[64];
    while(1){
        //get user input line
        fgets(line, LEN, stdin);
        line[strlen(line)-1] = 0;
        sscanf(line, "%s %s", command, pathname);

        printf("command is :%s\n", command);
        printf("pathname is: %s\n", pathname);
        /*
        //identify the command
        getBasename(pathname, dirname, basename);
        printf("dir name is -> %s\n", dirname);
        printf("base name is -> %s\n", basename);

        saveTree(root, "");
        fclose(fp);
       */ 
        if(!strcmp(command, "quit"))
            exit(0);
        //execute the command
        int cmd_idx = findCmd(command);
        funPtr[cmd_idx](pathname);

    }
    return 0;
}

#ifndef _TYPE_H
#define _TYPE_H
#define N 64
typedef struct node{
    char name[N];
    char node_t;
    struct node *childPtr,*siblingPtr,*parentPtr;
}NODE;

#endif

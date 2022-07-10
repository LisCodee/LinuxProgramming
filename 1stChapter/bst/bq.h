#ifndef _BQ_H
#define _BQ_H
typedef struct qe{
    struct qe* next;
    struct node* node;
}QE;

typedef struct node{
    int key;
    struct node *left, *right;
}NODE;

#endif

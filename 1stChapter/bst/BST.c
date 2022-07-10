#include<stdio.h>
#include<stdlib.h>
#include "bq.h"

#define N 7
int nodeValue[N] = {50,30,20,40,70,60,80};

//create a node

NODE* newNode(int key){
    NODE *node = (NODE*)malloc(sizeof(NODE));
    node->key = key;
    node->left = node->right = NULL;
}

NODE* insert(NODE* node, int key){
    if(!node){
        return newNode(key);
    }
    if(key > node->key)
        node->right = insert(node->right,key);
    else if(key < node->key)
        node->left = insert(node->left,key);
    return node;
}

//search bst for a give key
NODE* search(NODE* root, int key){
    if(!root || root->key == key)
        return root;
    if(key < root->key)
        return search(root->left, key);
    else
        return search(root->right, key);
}

void printLevel(NODE* root){
    if(!root)
        return;
    QE* queue=NULL;
    enqueue(&queue, root);
    NODE* node = dequeue(&queue);
    while(node){
        printf("%d->",node->key);
        if(node->left)
            enqueue(&queue, node->left);
        if(node->right)
            enqueue(&queue, node->right);
        node = dequeue(&queue);
    }
    printf("\n");
}

int main(){
    int i;
    NODE* root = NULL;
    root = insert(root, nodeValue[0]);
    for(i = 1; i < N;i++)
        insert(root, nodeValue[i]);
    printLevel(root);
    NODE* res = search(root,70);
    printf("find the node:%d\n",res->key);
    return 0;
}

#include"type.h"

NODE *mylist, node[N];//in bss section of program run-time image

int sumlist(NODE *p){
	return (p==0)?0:p->id+sumlist(p->next);
}

int printlist(NODE *p){
	while(p){
		printf("[%s %d]->",p->name, p->id);
		p = p->next;
	}
	printf("NULL\n");
}

int main(){
	int i;
	NODE *p;
	for(i = 0;i < N;i++){
		p = &node[i];
		sprintf(p->name, "%s%d","node",i);
		p->id = i;
		p->next = p+1;
	}
	node[N-1].next = 0;
	mylist = &node[0];
	printlist(mylist);
	printf("sum is %d\n",sumlist(mylist));
	return 0;
}

#include"createList.h"
node* createList(node array[],int n){
	node* head = &array[0];
	int i;
	for(i = 0;i < n;i++)
		if(i != n-1) array[i].next = &array[i+1];
	return head;
}
#include <iostream>
#include <cstring>
// #include "List.h"
#include "function.h"

Node* RotateList(Node* head, int k){
	int number[10000],ii = 0;
	memset(number,0,sizeof(number));
	Node* ptr = head;
	while(ptr != nullptr){
		number[ii++] = ptr->value;
		ptr = ptr->next;
	}
	Node* newHead = nullptr;
	ptr = newHead;
	for(int i = ii - 1;i >= k-1;i--){
		if(!newHead){
			newHead = new Node(number[i]);
			ptr = newHead;
		}else{
			ptr->next = new Node(number[i]);
			ptr = ptr->next;
		}
	}
	for(int i = 0;i < k-1;i++){
		ptr->next = new Node(number[i]);
		ptr = ptr->next;
	}
	return newHead;
}
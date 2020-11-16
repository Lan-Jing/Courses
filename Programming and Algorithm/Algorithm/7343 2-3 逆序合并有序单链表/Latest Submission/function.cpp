#include <iostream>
using namespace std;
#include "function.h"
#include <algorithm>

Node* ReverseMergeList(Node* List1, Node* List2){
	int number[10000],i = 0;
	Node* ptr = List1;
	while(ptr != nullptr) number[i++] = ptr->value,ptr = ptr->next;
	ptr = List2;
	while(ptr != nullptr) number[i++] = ptr->value,ptr = ptr->next;
	sort(number,number+i);
	Node* newHead = nullptr;
	ptr = newHead;
	for(int ii = i-1;ii >= 0;ii--){
		if(newHead == nullptr){
			newHead = new Node(number[ii]);
			ptr = newHead;
		}else{
			ptr->next = new Node(number[ii]);
			ptr = ptr->next;
		}
	}
	return newHead;
}
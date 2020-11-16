#include <iostream>
#include "List.h"

Node* List::SearchkthNode(int k){
//	cout<<"hello search"<<endl;
	Node* ptr = this->head;
	for(int i = 0;i < k-1;i++){
		if(ptr->next == nullptr) return nullptr;
		else ptr = ptr->next;
	}
	return ptr;
}

void List::InsertBack(Node* n){
//	cout<<"hello insert"<<endl;
	if(this->head == nullptr){
		this->head = n;
	}else{
		Node* ptr = this->head;
		while(ptr->next != nullptr) ptr = ptr->next;
		ptr->next = n;
	}
}

void List::DeleteElement(int k){
//	cout<<"hello delete"<<endl;
	Node* ptr = this->head;
	Node* prePtr = nullptr;
	for(int i = 1;i < k;i++){
		if(ptr->next == nullptr) return ;
		else{
			prePtr = ptr;
			ptr = ptr->next;
		}
	}
	if(ptr == this->head){
		this->head = ptr->next;
	}else{
		prePtr->next = ptr->next;
	}
	delete ptr;
}

void List::InsertAfterKth(Node* n, int k){
//	cout<<"hello insertafter"<<endl;
	if(!k){
		Node* ptr = this->head;
		this->head = n;
		this->head->next = ptr;
	}else{
		Node* prePtr = nullptr;
		Node* ptr = this->head;
		for(int i = 0;i < k;i++){
			if(ptr->next == nullptr && i != k-1) return ;
			else{
				prePtr = ptr;
				ptr = ptr->next;
			}
		}
		n->next = prePtr->next;
		prePtr->next = n;
	}
}
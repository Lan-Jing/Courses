#include <iostream>
using namespace std;
#include <cstdio>
#include <cstring>
#include "LinkedList.hpp"

LinkedList::LinkedList(){
    this->head = this->tail = NULL;
    this->_size = 0;
}
LinkedList::~LinkedList(){ this->clear(); this->head = this->tail = NULL; }
void LinkedList::add(E e){
    if(this->head == NULL){
        this->head = new node(e,NULL,NULL);
        this->tail = this->head;
    }else{
        node *newNode = new node(e,NULL,this->tail);
        this->tail->next = newNode;
        this->tail = newNode;
    }

    this->_size++;
}
void LinkedList::clear(){
    node *ptr1 = this->head, *ptr2 = this->head->next;
    while(ptr1 != NULL){
        delete ptr1;
        ptr1 = ptr2;
        if(ptr2 != NULL) ptr2 = ptr2->next;
    }
    this->head = this->tail = NULL;
    this->_size = 0;
}
bool LinkedList::contain(E e){ return this->indexOf(e) != -1; } 
bool LinkedList::isEmpty(){ return this->_size == 0; }
int LinkedList::size(){ return this->_size; }
void LinkedList::remove(E e){
    node *ptr = this->head, *ptrr;
    while(ptr != NULL){
        ptrr = ptr->next;
        if(ptr->data == e){
            if(ptr->next != NULL) ptr->next->prev = ptr->prev;
            else this->tail = ptr->prev;

            if(ptr->prev != NULL) ptr->prev->next = ptr->next;
            else this->head = ptr->next;
            
            delete ptr;
            this->_size--;
        }
        ptr = ptrr;
    }
}
E& LinkedList::get(int index){
    int count = 0; node *ptr = this->head;
    while(count < index) ptr = ptr->next,count++;
    return ptr->data;
}
E& LinkedList::operator [] (int index){ return this->get(index); }
int LinkedList::indexOf(E element){
    node *ptr = this->head;
    int count = 0;
    while(ptr != NULL)
        if(ptr->data == element) return count;
        else count++,ptr = ptr->next;
    return -1;
}
void LinkedList::sort(){
    E temp[this->_size + 5];
    memset(temp,0,sizeof(temp));

    node *ptr = this->head; int Aptr = 0;
    while(ptr != NULL) temp[Aptr++] = ptr->data, ptr = ptr->next;
    _sort(temp,0,this->_size -1);

    node *newHead = new node(temp[0],NULL,NULL);
    node *newTail = newHead;
    for(int i = 1;i <= this->_size - 1;i++){
        node *newNode = new node(temp[i],NULL,newTail);
        newTail->next = newNode;
        newTail = newNode;
    }

    this->clear();
    this->head = newHead; this->tail = newTail;
    this->_size = Aptr;
}
#include<iostream>
using namespace std;
#include "Stack.hpp"
#include<cstring>

int Stack::top() const {
    return this->p.data->num;
}
bool Stack::empty() const {
    return this->p.data == NULL ? true : false;
}
Stack::Stack(){
    this->p.data = NULL;
    this->p.count = new int(1);
}
Stack::~Stack(){
    this->clear();
    delete this->p.count;
}
void Stack::push(int value){
    if(*(this->p.count) > 1) copyOnWrite();

    node* temp = new node(value,this->p.data);
    this->p.data = temp;
}
void Stack::pop(){
    if(*(this->p.count) > 1) copyOnWrite();

    node* ptr = this->p.data;
    this->p.data = this->p.data->next;
    delete ptr;
}
void Stack::clear(){
    if(*(this->p.count) > 1) copyOnWrite();

    while(!(this->empty())) this->pop();
}
Stack::Stack(const Stack& other){
    this->p.data = other.p.data;
    this->p.count = other.p.count;
    (*(this->p.count))++;
}
Stack Stack::operator = (const Stack& other){
    if(other.p.data == this->p.data) return *this;
    
    (*(this->p.count))--;

    this->p.data = other.p.data;
    this->p.count = other.p.count;
    (*(this->p.count))++;
    return *this;
}
void Stack::copyOnWrite(){
    int temp[10010]; memset(temp,0,sizeof(temp)); int i = 0;
    node* ptr = this->p.data;
    while(ptr != NULL){
        temp[i++] = ptr->num; ptr = ptr->next;
    }

    (*(this->p.count)) -= 1;
    this->p.count = new int(1);
    
    this->p.data = NULL;
    for(int j = i-1;j >= 0;j--) this->push(temp[j]);
}
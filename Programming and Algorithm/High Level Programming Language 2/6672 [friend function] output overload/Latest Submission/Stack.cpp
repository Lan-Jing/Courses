#include<iostream>
using namespace std;
#include "Stack.hpp"
#include<cstring>

Stack::Stack(){
    this->data = NULL;
}
Stack::~Stack(){
    this->clear();
}
Stack::Stack(const Stack& other){
    this->data = NULL;

    node* ptr = other.data;
    int temp[10010]; memset(temp,0,sizeof(temp)); int i = 0;
    while(ptr != NULL){
        temp[i++] = ptr->num;
        ptr = ptr->next;
    }
    for(int j = i-1;j >= 0;j--) this->push(temp[j]);
}
void Stack::push(int value){
    if(this->empty()) this->data = new node(value,NULL);
    else{
        node* temp = new node(value,this->data);
        data = temp;
    }
}
void Stack::pop(){
    node* ptr = this->data;
    this->data = this->data->next;
    delete ptr;
}
int Stack::top() const {
    return this->data->num;
}
bool Stack::empty() const {
    return this->data == NULL ? true : false;
}
void Stack::clear(){
    while(!(this->empty())) this->pop();
    this->data = NULL;
}
Stack Stack::operator = (const Stack& other){
    if(this->data == other.data)
        return *this;
    this->clear();
    int temp[10010]; memset(temp,0,sizeof(temp)); int i = 0;
    node* ptr = other.data;
    while(ptr != NULL) temp[i++] = ptr->num,ptr = ptr->next;
    for(int j = i-1;j >= 0;j--) this->push(temp[j]);
    return *this;
}
std::ostream& operator <<(std::ostream& os, const Stack& s){
    Stack temp(s);
    os<<"[";
    int flag = 0;
    while(!temp.empty()){
        if(flag) os<<","; else flag = 1;
        os<<temp.top(); temp.pop(); 
    } 
    os<<"]";
    return os;
}
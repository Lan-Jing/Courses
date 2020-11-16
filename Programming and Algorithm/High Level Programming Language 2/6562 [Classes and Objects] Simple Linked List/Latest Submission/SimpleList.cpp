#include<iostream>
using namespace std;
#include<algorithm>
#include<cstring>
#include"SimpleList.hpp"

int list::size() const {
    return this->_size;
}
bool list::empty() const {
    return size() ? false : true;
}
string NumToString(int num){
    if(!num) return "0"; //注意这个算法里面0要特殊处理
    string result;
    while(num) result += num%10+'0',num /= 10;
    reverse(result.begin(),result.end());
    return result;
}
string list::toString() const {
    node *ptr = head;
    string result;
    while(ptr != NULL){
        result += NumToString(ptr->data) + "->";
        ptr = ptr->next;
    }
    result += "NULL";
    return result;
}
void list::erase(int position){
    if(!size() || position >= size()) return ;//要判断插入和删除操作的参数是否是合法的
    node* ptr = head;
    if(!position){
        head = head->next;
        delete ptr;
    }else{
        for(int i = 0;i < position-1;i++) ptr = ptr->next;
        node* temp = ptr->next;
        ptr->next = ptr->next->next;
        delete temp;
    }
    _size--;
}
list::list(){
    head = NULL; 
    this->_size = 0;
}
list::~list(){
    while(head != NULL) erase(0);
    _size = 0;
}
list& list::operator = (const list& other){
    while(head != NULL) erase(0);

    node* ptr = other.head;
    for(int i = 0;ptr != NULL;i++){
        insert(i,ptr->data);
        ptr = ptr->next;
    }
    _size = other._size;
    return *this;
}
list::list(const list& other){
    head = NULL; _size = 0; 
    *this = other;
}
void list::insert(int position,const int& data){
    if(position > size()) return ;
    
    if(!size()){
        head = new node;
        head->data = data;
        head->next = NULL;
    }else if(!position){
        node* nnode = new node;
        nnode->data = data;
        nnode->next = head;
        head = nnode;
    }else{
        node* nnode = new node;
        node* ptr = head;
        
        for(int i = 0;i < position-1;i++) ptr = ptr->next;
        nnode->data = data;
        nnode->next = ptr->next;
        ptr->next = nnode;
    }
    
    _size++;
}
list& list::sort(){
    int temp[100010]; memset(temp,0,sizeof(temp));
    int loc = 0,length = size();
    
    node* ptr = head;
    while(ptr != NULL){
        temp[loc++] = ptr->data;
        ptr = ptr->next;
    }
//    for(int i = 0;i < length;i++) cout<<temp[i]<<' '; cout<<endl;
   
    while(head != NULL) erase(0);
    
    std::sort(temp,temp+length);

    for(int i = 0;i < length;i++)
        insert(i,temp[i]);
    return *this;
}
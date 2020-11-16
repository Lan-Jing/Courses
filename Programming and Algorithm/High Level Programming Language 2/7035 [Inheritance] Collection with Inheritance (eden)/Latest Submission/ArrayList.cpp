#include <iostream>
using namespace std;
#include <cstdio>
#include <cstring>
#include "ArrayList.hpp"

ArrayList::ArrayList(){
    this->storage = new E[1];
    this->_size = 0,this->_maxsize = 1;
}
ArrayList::~ArrayList(){ this->clear(); }
bool ArrayList::isEmpty(){ return this->_size == 0; }
int ArrayList::indexOf(E element){
    for(int i = 0;i < this->_size;i++)
        if(this->storage[i] == element) return i;
    return -1;
}
int ArrayList::size() { return this->_size; }
void ArrayList::clear(){ delete [] this->storage, this->storage = NULL; this->_size = this->_maxsize = 0; }
E& ArrayList::get(int index){ return this->storage[index]; }
E& ArrayList::operator [] (int index){ return this->get(index); }
bool ArrayList::contain(E e){ return this->indexOf(e) != -1; }
void ArrayList::remove(E e){
//    cout<<this->_maxsize<<' '<<this->_size<<endl;

    E tempList[this->_maxsize];
    int ptr = 0;
    for(int i = 0;i < this->_size;i++)
        if(this->storage[i] != e) tempList[ptr++] = this->storage[i];
    this->_size = ptr;
    for(int i = 0;i < this->_size;i++) this->storage[i] = tempList[i];
}
void ArrayList::add(E e){
    if(this->_size == this->_maxsize){
        this->_maxsize *= this->extend_factor;
        E *newList = new E[this->_maxsize]; int size = this->_size;
        for(int i = 0;i < this->_size;i++) newList[i] = this->storage[i];
        
        int maxsize = this->_maxsize;
        this->clear();
        this->storage = newList;
        this->_size = size;
        this->_maxsize = maxsize;
    }
    this->storage[this->_size++] = e;
}
void ArrayList::sort(){
    E temp[this->_size + 5];
    memset(temp,0,sizeof(temp));
    for(int i = 0;i < this->_size;i++) temp[i] = this->storage[i];
    _sort(temp,0,this->_size-1);
    for(int i = 0;i < this->_size;i++) this->storage[i] = temp[i];
}

void _sort(E *arr,int lbound, int rbound){
    if(lbound >= rbound) return ;
    E pivot = arr[rbound];
    int lptr = lbound;
    for(int rptr = lptr;rptr < rbound;rptr++)
        if(arr[rptr] < pivot) swap(arr[rptr],arr[lptr++]);
    swap(arr[lptr],arr[rbound]);

    _sort(arr,lbound,lptr - 1); _sort(arr,lptr + 1,rbound);
}
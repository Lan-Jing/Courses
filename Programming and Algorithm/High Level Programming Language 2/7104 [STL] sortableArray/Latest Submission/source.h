#include <iostream>
using namespace std;
#include <cstring>
#include <cstdio>

template<typename elementType>
void _sort(elementType *arr,int lbound,int rbound){
    if(lbound >= rbound) return ;
    elementType pivot = arr[rbound];
    int i = lbound;
    for(int lptr = lbound;lptr < rbound;lptr++)
        if(arr[lptr] < pivot) swap(arr[i++],arr[lptr]);
    swap(arr[i],arr[rbound]);

    _sort(arr,lbound,i-1); _sort(arr,i+1,rbound);
}

template<typename elementType>
class SortableArray {
public:
    SortableArray(int maxSize);
    void pushBack(elementType);
    void sort();
    void printArray();
private:
    elementType *arr;
    int maxSize,size;
};

template<typename elementType>
SortableArray<elementType>::SortableArray(int maxSize){ 
    this->arr = new elementType[maxSize]; 
    this->maxSize = maxSize;
    this->size = 0;
}

template<typename elementType>
void SortableArray<elementType>::printArray(){
    for(int i = 0;i < this->size;i++) cout<<this->arr[i]<<' '; 
    cout<<endl;
}

template<typename elementType>
void SortableArray<elementType>::pushBack(elementType temp){
    if(this->size == this->maxSize){
        this->maxSize *= 2;
        elementType *newarr = new elementType[this->maxSize];
        for(int i = 0;i < this-> size;i++)
            newarr[i] = this->arr[i];
        delete this->arr;
        this->arr = newarr;
    }
    this->arr[this->size++] = temp;
}

template<typename elementType>
void SortableArray<elementType>::sort(){ _sort(this->arr,0,this->size-1); }
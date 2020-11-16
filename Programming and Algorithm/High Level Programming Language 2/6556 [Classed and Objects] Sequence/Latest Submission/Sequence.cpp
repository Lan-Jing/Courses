#include<iostream>
using namespace std;
#include<cstring>
#include"Sequence.h"

Sequence::Sequence(){
    a = new int[1010];
    msize = 0;
    mempty = 1;
}
Sequence::~Sequence(){
    delete [] a;
    msize = 0;
    mempty = 1;
}
int Sequence::size(){
    return msize;
}
bool Sequence::empty(){
    return mempty ? true : false;
}
bool Sequence::find(int value){
    int tail = size();
    for(int i = 0;i < tail;i++)
        if(a[i] == value)
            return true;
    return false;
}
int& Sequence::at(int pos){
    return a[pos];
}
int Sequence::front(){
    return a[0];
}
int Sequence::back(){
    return a[size()-1];
}
void Sequence::insert(int value){
    a[size()] = value;
    msize++;
    if(msize) mempty = 0;
}
void Sequence::insert(int pos,int value){
    int tail = size();
    for(int i = tail;i > pos;i--) a[i] = a[i-1];
    a[pos] = value;
    msize++; 
    if(msize) mempty = 0;
}
void Sequence::clear(){
    memset(a,0,sizeof(a));
    msize = 0; mempty = 1;
}
void Sequence::reverse(){
    int length = size();
    for(int i = 0,j = length-1;i <= j;i++,j--){
        int temp = a[i];
        a[i] = a[j];
        a[j] = temp;
    }
}
void Sequence::reverse(int fir,int las){
    int length = size();
    for(int i = fir,j = las-1;i <= j;i++,j--){
        int temp = a[i];
        a[i] = a[j];
        a[j] = temp;
    }
}
void Sequence::replace(int value1,int value2){
    int length = size();
    for(int i = 0;i < length;i++)
        if(a[i] == value1) a[i] = value2;
}
void Sequence::swap(Sequence &seq2){
    int temp[1010]; memset(temp,0,sizeof(temp));
    int l1 = size(),l2 = seq2.size();
    for(int i = 0;i < l1;i++)
        temp[i] = a[i];
    clear();
    for(int i = 0;i < l2;i++)
        insert(seq2.a[i]);
    seq2.clear();
    for(int i = 0;i < l1;i++)
        seq2.insert(a[i]);/*注意这里用l1\l2不能使用size()，因为clear已经修改了原列表的大小*/
}
#ifndef MYDEQUE_HPP
#define MYDEQUE_HPP
#include <iostream>
using namespace std;
#include <cstring>
#include "myIterator.hpp"

#define blockLength 128
#define expandFactor 2

class mydeque {
public:
    mydeque();
    mydeque(const mydeque& other);
    ~mydeque();

    void push_back(int num);
    void push_front(int num);
    void insert(myIterator pos, int num);
    void erase(myIterator pos);
    void clear();

    int& operator[](int pos);
    int front();
    int back();
    int size();
    
    int showHeadOffset(){ return this->headOffset; }
    int showHeadPosition(){ return this->headPosition; }

    void operator = (const mydeque& other);

    myIterator begin();
    myIterator end();
    myIterator rbegin();
    myIterator rend();

private:
    int headOffset,tailOffset;
    int headPosition, tailPosition;
    int totalCount,totalBlock;
    int** deque;
      
    void expandAndRearrange();

};

#endif
#ifndef COLLECTION_H
#define COLLECTION_H

#include <iostream>
using namespace std;

#define E int 

class Collection {
public:
    virtual void add(E e) = 0;
    virtual void clear(void) = 0;
    virtual bool contain(E e) = 0;
    virtual bool isEmpty(void) = 0;
    virtual void remove(E e) = 0;
    virtual int size(void) = 0;
    virtual ~Collection(){};
};

#endif
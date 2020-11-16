#ifndef LIST_H
#define LIST_H

#include <iostream>
using namespace std;
#include "Collection.hpp"

class List : public Collection {
public:
    virtual E& get(int index) = 0;
    virtual E& operator [] (int index) = 0;
    virtual int indexOf(E element) = 0;
    virtual void sort() = 0;
};

void _sort(E *arr,int lbound,int rbound);

#endif
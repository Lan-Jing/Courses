#ifndef MYITERATOR_HPP
#define MYITERATOR_HPP
#include <iostream>
using namespace std;

#define blockLength 128

class myIterator {
public:
    myIterator(int** target,int mode,int dequePos,int blockPos){
        this->setPtr(target);
        this->setMode(mode);
        this->setDequePos(dequePos);
        this->setBlockPos(blockPos);
    }
    myIterator& operator = (const myIterator &iter);
    myIterator& operator ++ ();
    myIterator& operator ++ (int);
    myIterator operator + (int num);
    bool operator != (const myIterator &iter);
    bool operator == (const myIterator &iter);
    int& operator * ();

    int** showPtr() const { return this->ptr; }
    int showDequePos() const { return this->dequePos; }
    int showBlockPos() const { return this->blockPos; }
    int showMode() const { return this->reverseOrNot; }

    void setPtr(int** target){ this->ptr = target; }
    void setMode(int mode){ this->reverseOrNot = mode; }
    void setDequePos(int newDequePos){ this->dequePos = newDequePos; };
    void setBlockPos(int newBlockPos){ this->blockPos = newBlockPos; };

private:
    int** ptr;
    int dequePos,blockPos;
    int reverseOrNot;
};

#endif 

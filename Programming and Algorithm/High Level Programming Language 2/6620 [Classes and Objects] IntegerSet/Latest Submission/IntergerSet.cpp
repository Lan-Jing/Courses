#include<iostream>
using namespace std;
#include "IntegerSet.h"
#include<string.h>

void IntegerSet::emptySet(){
    memset(set,0,sizeof(set));
}
IntegerSet::IntegerSet(int s[],int l){
    emptySet();
    for(int i = 0;i < l;i++)
        insertElement(s[i]);
}
IntegerSet IntegerSet::unionOfSets(const IntegerSet& other){
    IntegerSet result;
    for(int i = 0;i <= 100;i++)
        if(set[i] || other.set[i])
            result.insertElement(i);
    return result;
}
IntegerSet IntegerSet::intersectionOfSets(const IntegerSet& other){
    IntegerSet result;
    for(int i = 0;i <= 100;i++)
        if(set[i] && other.set[i])
            result.insertElement(i);
    return result;
}
void IntegerSet::insertElement(int element){
    if(validEntry(element)) set[element] = 1;
    else cout<<"Invalid insert attempted!\n";
}
void IntegerSet::deleteElement(int element){
    if(validEntry(element) && set[element])
        set[element] = 0;
    else cout<<"Invalid delete attempted\n";
}
bool IntegerSet::isEqualTo(const IntegerSet& other) const {
    for(int i = 0;i <= 100;i++)
        if(set[i] != other.set[i])
            return false;
    return true;
}
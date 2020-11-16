#include<iostream>
using namespace std;
#include<cstring>
#include"SingleDog.h"

int SingleDog::count = 0;
SingleDog::SingleDog(int id_, char* name_) :id(id_) {
    count++;
    name = new char[20];
    strcpy(name,name_);//Deep copy
}
SingleDog::SingleDog(const SingleDog &other) :id(other.id) {
    count++;
    name = new char[20];
    strcpy(name,other.name);
}
SingleDog::~SingleDog(){
    count--;
    delete [] name;
}
int SingleDog::getCount(){
    return count;
}
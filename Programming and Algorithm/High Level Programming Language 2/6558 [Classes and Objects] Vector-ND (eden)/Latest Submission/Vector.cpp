#include<iostream>
using namespace std;
#include"Vector.h"

Vector::Vector(string name_,int dimension_,int param_[]){
    name = name_;
    dimension = dimension_;
    param = new int[1000];
    for(int i = 0;i < dimension_;i++)
        param[i] = param_[i];
    cout<<"construct a vector called "<<name<<'.'<<endl;
}
Vector::Vector(const Vector &otherVec){
    name = otherVec.name;
    dimension = otherVec.dimension;
    param = new int[1000];
    for(int i = 0;i < dimension;i++)
        param[i] = otherVec.param[i];
    cout<<"copy a vector called "<<name<<'.'<<endl;
}
Vector::~Vector(){
    cout<<"release memory from a vector called "<<name<<'.'<<endl;
    name = "";
    dimension = 0;
    delete [] param;
    param = NULL;
}
void Vector::isEqual(const Vector &other){
    int same_value = 0;
    int same_name = 0;
    if(name == other.name) same_name = 1;
    if(dimension == other.dimension){
        int flag = 1;
        for(int i = 1;i <= dimension;i++)
            if(param[i-1] != other.param[i-1]){
                flag = 0; break;
            }
        if(flag) same_value = 1;
    }
    printf("%s, %s\n",same_name ? "same name" : "different name",same_value ? "same value." :"different value.");
}
void Vector::setName(string str){
    name = str;
}
void Vector::print(){
    cout<<name<<'(';
    for(int i = 1;i <= dimension;i++){
        cout<<param[i-1];
        if(i < dimension) cout<<", ";
    }
    cout<<')'<<endl;
}
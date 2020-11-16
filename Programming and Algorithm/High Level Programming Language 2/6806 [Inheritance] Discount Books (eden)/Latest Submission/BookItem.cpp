#include<iostream>
using namespace std; 
#include "BookItem.hpp"

BookItem::BookItem(const string& bookName, double salesPrice){
    this->price_ = salesPrice;
    this->name_ = bookName;
}
string BookItem::getName() const {
    return this->name_;
}
double BookItem::netPrice(int cnt) const {
    return cnt* this->price_;
}
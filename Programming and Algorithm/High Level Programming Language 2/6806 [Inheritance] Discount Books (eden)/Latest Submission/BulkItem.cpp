#include<iostream>
using namespace std;
#include "BulkItem.hpp"

BulkItem::BulkItem(const string& bookName, double salesPrice, 
                   int qty, double salesDisc)
                   :BookItem(bookName,salesPrice) {
    this->minQty_ = qty;
    this->discount_ = salesDisc;
} 
double BulkItem::netPrice(int cnt) const {
    if(cnt > this->minQty_) return cnt * this->price_ * this->discount_;
    else return cnt * this->price_;
}
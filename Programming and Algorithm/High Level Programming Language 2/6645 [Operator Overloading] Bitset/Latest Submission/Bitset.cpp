#include<iostream>
using namespace std;
#include "Bitset.h"
#include<cstring>

bitset::bitset(){
    memset(a,0,sizeof(a));
}
bool bitset::any() const {
    for(int i = 0;i < 5;i++) 
        if(this->a[i]) return true;
    return false;
}
bool bitset::none() const {
    return this->any() ? false : true;
}
bool bitset::all() const {
    for(int i = 0;i < 5;i++)
        for(int j = 0;j < 32;j++)
            if((this->a[i] & (1<<j)) == 0) return false;
    return true;
} 
int bitset::count() const {
    int count = 0;
    for(int i = 0;i < 5;i++)
        for(int j = 0;j < 32;j++)
            if(this->a[i] & (1<<j)) count++;
    return count;
}
void bitset::set(int pos){
    int pos1 = pos/32;
    int pos2 = pos%32;
    a[pos1] |= (1<<pos2);
}
void bitset::reset(int pos){
    int pos1 = pos/32;
    int pos2 = pos%32;
    a[pos1] &= (~(1<<pos2));
}
bool bitset::test(int pos) const {
    if(pos >= 5*32 || pos < 0) return false;
    int pos1 = pos/32;
    int pos2 = pos%32;
    int temp = this->a[pos1];
    if((temp & (1<<pos2)) != 0) return true;
    else return false;
}
bool bitset::operator == (const bitset& b) const {
    for(int i = 0;i < 5;i++)
        if(this->a[i] != b.a[i]) return false;
    return true;
}
bool bitset::operator != (const bitset& b) const {
    return (*this == b) ? false : true;
}
bool bitset::operator [] (int pos) const {
    return this->test(pos);
}
bitset& bitset::operator = (const bitset& b){
    for(int i = 0;i < 5;i++)
        this->a[i] = b.a[i];
    return *this;
}
bitset bitset::operator ~() const {
    bitset temp;
    for(int i = 0;i < 5;i++) temp.a[i] = ~this->a[i];
    return temp;
}
bitset bitset::operator & (const bitset& b) const{
    bitset temp;
    for(int i = 0;i < 5;i++) temp.a[i] = this->a[i] & b.a[i];
    return temp;
}
bitset bitset::operator | (const bitset& b) const {
    bitset temp;
    for(int i = 0;i < 5;i++) temp.a[i] = this->a[i] | b.a[i];
    return temp;
} 
bitset bitset::operator ^ (const bitset& b) const {
    bitset temp;
    for(int i = 0;i < 5;i++) temp.a[i] = this->a[i] ^ b.a[i];
    return temp;
}
bitset& bitset::operator &= (const bitset& b){
    *this = *this & b;
    return *this;
}
bitset& bitset::operator |= (const bitset& b){
    *this = *this | b;
    return *this;
}
bitset& bitset::operator ^= (const bitset& b){
    *this = *this ^ b;
    return *this;
}
bitset bitset::operator << (int pos) const {
    if(!pos) return *this;
    bitset temp;
    for(int i = 4;i >= 0;i--)
        for(int j = 31;j >= 0;j--)
            if(this->test(32*i+j-pos))
                temp.set(32*i+j);
            else temp.reset(32*i+j);
    return temp;
}
bitset bitset::operator >> (int pos) const {
    if(!pos) return *this;
    bitset temp;
    int flag = this->test(32*N-1);
    for(int i = 0;i < 5;i++)
        for(int j = 0;j < 32;j++)
            if(this->test(32*i+j+pos))
                temp.set(32*i+j);
            else temp.reset(32*i+j);
    if(flag) for(int i = 31;i >= 32-pos;i--) temp.set(32*4+i);
    return temp;
}
bitset& bitset::operator <<= (int pos) {
    *this = *this << pos;
    return *this;
}
bitset& bitset::operator >>= (int pos) {
    *this = *this >> pos;
    return *this;
}
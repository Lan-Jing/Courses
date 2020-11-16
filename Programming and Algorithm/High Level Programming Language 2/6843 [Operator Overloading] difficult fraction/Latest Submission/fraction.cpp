#include<iostream>
using namespace std;
#include "fraction.hpp"

long long gcdd(long long a,long long b){
    return !b ? a : gcdd(b,a%b);
}
void fraction::simp(){
    if(this->_denominator < 0) this->_denominator *= -1,this->_numerator *= -1;
}
fraction::fraction(const int& num,const int& den){
    this->_numerator = num,this->_denominator = den;
    this->simp();
}
fraction::fraction(const fraction& other){
    (*this) = other;
}
void fraction::operator = (const fraction& other){
    this->_numerator = other._numerator;
    this->_denominator = other._denominator;
}
fraction fraction::operator + (const fraction& other) const {
    fraction result;
    if(!this->_denominator || !other._denominator){
        result._numerator = result._denominator = 0;
        return result;
    }
    long long a = this->_numerator,b = this->_denominator,
              c = other._numerator,d = other._denominator;
    long long new_num = a*d+b*c,
              new_den = b*d;
    result._numerator = new_num / gcdd(new_num,new_den);
    result._denominator = new_den / gcdd(new_num,new_den);
    result.simp();
    return result;
}
fraction fraction::operator - (const fraction& other) const {
    fraction result;
    if(!this->_denominator || !other._denominator){
        result._numerator = result._denominator = 0;
        return result;
    }
    long long a = this->_numerator,b = this->_denominator,
              c = other._numerator,d = other._denominator;
    long long new_num = a*d-b*c,
              new_den = b*d;
    result._numerator = new_num / gcdd(new_num,new_den);
    result._denominator = new_den / gcdd(new_num,new_den);
    result.simp();
    return result;
}
fraction fraction::operator * (const fraction& other) const {
    fraction result;
    if(!this->_denominator || !other._denominator){
        result._numerator = result._denominator = 0;
        return result;
    }
    long long a = this->_numerator,b = this->_denominator,
              c = other._numerator,d = other._denominator;
    long long new_num = a*c,
              new_den = b*d;
    result._numerator = new_num / gcdd(new_num,new_den);
    result._denominator = new_den / gcdd(new_num,new_den);
    result.simp();
    return result;
}
fraction fraction::operator / (const fraction& other) const {
    fraction result;
    if(!this->_denominator || !other._denominator || !other._numerator){
        result._numerator = result._denominator = 0;
        return result;
    }
    long long a = this->_numerator,b = this->_denominator,
              c = other._numerator,d = other._denominator;
    long long new_num = a*d,
              new_den = b*c;
    result._numerator = new_num / gcdd(new_num,new_den);
    result._denominator = new_den / gcdd(new_num,new_den);
    result.simp();
    return result;
}
void fraction::operator += (const fraction& other){
    (*this) = (*this) + other;
}
void fraction::operator -= (const fraction& other){
    (*this) = (*this) - other;
}
void fraction::operator *= (const fraction& other){
    (*this) = (*this) * other;
}
void fraction::operator /= (const fraction& other){
    (*this) = (*this) / other;
}
bool fraction::operator == (const fraction& other) const {
    if(this->_denominator == 0 && other._denominator == 0) return true;
    else if(this->_denominator == other._denominator && 
            this->_numerator == other._numerator) return true;
    else return false;
}
bool fraction::operator != (const fraction& other) const {
    return !((*this) == other);
}
bool fraction::operator < (const fraction& other) const {
    fraction result = (*this) - other;
    if(result._numerator < 0) return true;
    else return false;
}
bool fraction::operator > (const fraction& other) const {
    fraction result = (*this) - other;
    if(result._numerator > 0) return true;
    else return false;
}
bool fraction::operator <= (const fraction& other) const {
    fraction result = (*this) - other;
    if(result._numerator <= 0) return true;
    else return false;
}
bool fraction::operator >= (const fraction& other) const {
    fraction result = (*this) - other;
    if(result._numerator >= 0) return true;
    else return false;
}
istream& operator >> (istream& in, fraction& frac){
    in>>frac._numerator;
    in>>frac._denominator;
    return in;
}
ostream& operator << (ostream& out,const fraction& frac){
    if(frac._denominator == 0) out<<"NaN";
    else if(gcdd(frac._numerator,frac._denominator) == frac._denominator)
        out<<frac._numerator/frac._denominator;
    else out<<frac._numerator<<"/"<<frac._denominator;
    return out;
}
#include<iostream>
using namespace std;
#include "Complex.hpp"

Complex::Complex(double Real,double Imag){
    SetReal(Real); SetImag(Imag);
}
Complex::Complex(const Complex& other){
    *this = other;
}
void Complex::operator =(const Complex& other){
    SetReal(other.getReal());
    SetImag(other.getImag());
}
bool Complex::operator ==(const Complex& other) const {
    if(real == other.getReal() && imag == other.getImag())
        return true;
    else return false;
}
bool Complex::operator !=(const Complex& other) const {
    return !(*this==other);
}
Complex Complex::operator -(){
    Complex result;
    result.SetReal(-getReal());
    result.SetImag(-getImag());
    return result;
}
Complex Complex::operator +(const Complex& other) const {
    Complex result;
    result.SetReal(this->getReal()+other.getReal());
    result.SetImag(this->getImag()+other.getImag());
    return result;
}
Complex Complex::operator -(Complex& other) const {
    return (*this) + (-other);
}
Complex Complex::operator *(const Complex& other) const {
    Complex result;
    double a = this->getReal(),b = this->getImag();
    double c = other.getReal(),d = other.getImag();
    result.SetReal(a*c-b*d);
    result.SetImag(a*d+b*c);
    return result;
}
Complex Complex::operator /(const Complex& other) const {
    Complex result;
    double a = this->getReal(),b = this->getImag();
    double c = other.getReal(),d = other.getImag();
    result.SetReal((a*c+b*d)/(c*c+d*d));
    result.SetImag((b*c-a*d)/(c*c+d*d));
    return result;
}
void Complex::operator +=(const Complex& other){
    *this = *this + other;
}
void Complex::operator -=(Complex& other){
    *this = *this - other;
}
void Complex::operator *=(const Complex& other){
    *this = *this * other;
}
void Complex::operator /=(const Complex& other){
    *this = *this / other;
}
//注意 const 函数和 const 参数的问题！
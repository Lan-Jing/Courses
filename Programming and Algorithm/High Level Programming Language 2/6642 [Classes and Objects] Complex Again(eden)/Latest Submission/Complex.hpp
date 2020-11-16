#include<iostream>
using namespace std;
#include<fstream>

class Complex {

    //A friend function to print Complex numbers like a+bi where a is the real part and b is the imaginary part
  friend ostream& operator <<(ostream& out, const Complex& c) {
    if (c.real == 0 && c.imag != 0) {
           out << c.imag << 'i' << endl;
           return out;
         }
         out << c.real;
         if (c.imag != 0) {
           if (c.imag > 0)
             out << "+" << c.imag;
           else
             out << c.imag;
           out << 'i' << endl;
         }
         return out;
  }


public:
    Complex(double = 0.0, double = 0.0);
    Complex(const Complex&);
    Complex operator -();
    void operator =(const Complex&);
    Complex operator +(const Complex&) const ;
    Complex operator -(Complex&) const ;
    Complex operator *(const Complex&) const ;
    Complex operator /(const Complex&) const ;
    void operator +=(const Complex&);
    void operator -=(Complex&);
    void operator *=(const Complex&);
    void operator /=(const Complex&);
    bool operator ==(const Complex&) const ;
    bool operator !=(const Complex&) const ;

    double getReal() const {return real;}
    double getImag() const {return imag;}
    void SetReal(double re){real = re;}
    void SetImag(double im){imag = im;}
private:
   double real; 
   double imag; 
};
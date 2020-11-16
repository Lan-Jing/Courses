#include<iostream>
#include<string>
#include"1002.h"
using namespace std;

ostream& operator<<(ostream& os, const Date& date)
{
  os << date.getYear() << "-" << date.getMonth() << "-" << date.getDay();
  return os;
}

/* 
f1() test for:
  constructor, copy constructor, assign assignment
  static member function leapyear()
  operator ==, !=, <, <=, >, >=
*/
void f1()
{
  Date date1, date2(2003,1,1);
  Date date3 = Date(2007,2,28);
  cout << "date1: " << date1 << endl;
  cout << "date2: " << date2 << endl;
  
  cout << "date3 after copy constructor: " << date3 << endl;
  date3 = date1;
  cout << "date3 after copy asignment: " << date3 << endl;  

  cout << "year 1996 is leap year? " << Date::leapyear(1996) << endl;
  cout << "year 1200 is leap year? " << Date::leapyear(1200) << endl;
  cout << "year 1300 is leap year? " << Date::leapyear(1300) << endl;
  cout << "year 1999 is leap year? " << Date::leapyear(1999) << endl;

  cout <<  "(date1==date3)? " << (date1==date3) << endl;
  cout <<  "(date1!=date3)? " << (date1!=date3) << endl;
  cout <<  "(date1==date2)? " << (date1==date2) << endl;
  cout <<  "(date1!=date2)? " << (date1!=date2) << endl;

  cout <<  "(date1<date1)? " << (date1<date1) << endl;
  cout <<  "(date1<=date1)? " << (date1<=date1) << endl;
  cout <<  "(date1<date2)? " << (date1<date2) << endl;
  cout <<  "(date1<=date2)? " << (date1<=date2) << endl;
  
  cout <<  "(date1>date1)? " << (date1>date1) << endl;
  cout <<  "(date1>=date1)? " << (date1>=date1) << endl;
  cout <<  "(date1>date2)? " << (date1>date2) << endl;
  cout <<  "(date1>=date2)? " << (date1>=date2) << endl;

}
/*
f2() test for:
  subscript opeartor [] as both lvalue and rvalue
*/
void f2()
{
  Date date1(2011,4,1);
  cout << "date1: " << date1 << endl;
  cout << date1["year"] << endl;
  cout << date1["month"] << endl;
  cout << date1["day"] << endl;  
  date1["year"] = 2000;
  date1["month"] = 10;
  date1["day"] = 10;
 
  cout << "date1: " << date1 << endl;
}

/*
f3_0() test for:
  operator 
  +=, -=, 
  +, -, 
*/
void f3_0(const Date& d)
{
  Date date = d;

  cout << "date = " << date << endl;
  cout << "date+1 = " << date+1 << endl;
  cout << "date-1 = " << date-1 << endl;

  date+=366;
  cout << "date = " << date << endl;
  date-=365;
  cout << "date = " << date << endl;
  date-=-365;
  cout << "date = " << date << endl;
  date+=-366;
  cout << "date = " << date << endl;
      
  cout << endl;
}
/*
f3_1() test for:
  operator 
  ++(prefix), --(prefix), 
  ++(postfix), --(postfix), 
  +=, -=, 
  +, -, 
  <<
*/

void f3_1(const Date& d)
{
  Date date = d;
  cout << "date = " << date << endl;
  cout << "++date = " << ++date << endl;
  cout << "--date = " << --date << endl;
  cout << "date++ = " << date++ << endl;
  cout << "date-- = " << date-- << endl;
  cout << "date = " << date << endl;
  date+=366;
  cout << "date = " << date << endl;
  date-=365;
  cout << "date = " << date << endl;
  date-=-365;
  cout << "date = " << date << endl;
  date+=-366;
  cout << "date = " << date << endl;
      
  cout << endl;
}

void f3()
{
  Date date1, date2(2004,2,28), date3(2007,2,28),date4(2000,2,28),date5(99,12,31);
  f3_0(date1);
  f3_0(date2);
  f3_0(date3);
  f3_0(date4);
  f3_0(date5);
}


int main()
{
  f3();
  //system("PAUSE");
  return 0;
}




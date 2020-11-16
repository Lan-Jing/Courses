#include<iostream>
#include<string>
#include"1001.h"
using namespace std;

ostream& operator<<(ostream& os, const Date& date)
{
  os << date.getYear() << "-" << date.getMonth() << "-" << date.getDay();
  return os;
}



/*
f3_1() test for:
  operator 
  ++(prefix), --(prefix), 
  ++(postfix), --(postfix), 
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

  cout << endl;
}



void f3()
{
  Date date1, date2(2004,2,28), date3(2007,2,28),date4(2000,2,28),date5(99,12,31);
  f3_1(date1);
  f3_1(date2);
  f3_1(date3);
  f3_1(date4);
  f3_1(date5);
}


int main()
{
  f3();
  //system("PAUSE");
  return 0;
}




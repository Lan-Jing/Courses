// Problem#: 17893
// Submission#: 4697291
// The source code is licensed under Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License
// URI: http://creativecommons.org/licenses/by-nc-sa/3.0/
// All Copyright reserved by Informatic Lab of Sun Yat-sen University
#include <iostream>
#include <string>
int num[13] = {31, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
using namespace std;
class Date {
 public:
  Date(int y=0, int m=1, int d=1);
  static bool leapyear(int year);
  int getYear() const;
  int getMonth() const;
  int getDay() const;
  Date operator ++();
  Date operator ++(int);
  Date operator --();
  Date operator --(int);
  Date operator +=(int);
  Date operator +(int);
  Date operator -=(int);
  Date operator -(int);
  int & operator [](string);
  friend ostream& operator<<(ostream&, const Date&);
  int equal(Date &k);
  bool operator <(Date &k);
  bool operator <=(Date &k);
  bool operator ==(Date &k);
  bool operator !=(Date &k);
  bool operator >(Date &k);
  bool operator >=(Date &k);
 private:
  int year, month, day;
};
int & Date :: operator [](string s) {
 if (s == "year") return year;
 if (s == "month") return month;
 if (s == "day") return day;
}


bool Date:: leapyear(int year) {
 if ((year % 100 != 0 && year %4 == 0) || year % 400 == 0) return true;
 else
  return false;
}
Date :: Date(int y, int m, int d) {
 year = y;
 month = m;
 day = d;
}
int Date:: getYear() const {
 return year;
}
int Date:: getMonth() const {
 return month;
}
int Date:: getDay() const {
 return  day;
}
Date Date::operator ++() {
 *this = *this + 1;
 return *this;
}
Date Date::operator ++(int k) {
 Date temp = *this;
 *this = *this + 1;
 return temp ;
}
Date Date::operator --() {
 *this = *this - 1;
 return *this;
}
Date Date::operator --(int k) {
 Date temp = *this;
 *this = *this - 1;
 return temp;
}
Date Date::operator +=(int k) {
 *this = *this + k;
 return *this;
}
Date Date::operator -=(int k) {
 *this = *this - k;
 return *this;
}
Date Date :: operator + (int k) {
 if (k < 0) return (*this - (-k));
 Date m = *this;
 m.day+= k;
 if (leapyear(m.year)) num[2] = 29;
 else num[2] = 28;
 while (m.day > num[m.month]) {
  m.day-= num[m.month];
  ++m.month;
  if (m.month > 12) {
   m.month = 1;
   ++m.year;
  }
  if (leapyear(m.year)) num[2] = 29;
  else num[2] = 28;
 }
 return m;
}
Date Date :: operator - (int k) {
 if (k < 0) return (*this + (-k));
 Date m = *this;
 m.day-= k;
 if (leapyear(m.year)) num[2] = 29;
 else num[2] = 28;
 while (m.day <= 0) {
  --m.month;
  m.day+= num[m.month];
  if (m.month == 0) {
   m.month = 12;
   --m.year;
  }
  if (leapyear(m.year)) num[2] = 29;
  else num[2] = 28;
 }
 return m;
}

int Date::equal(Date &k) {
 if (k.year != year) return (year - k.year);
 else if (k.month != month) return (month - k.month);
 else if (k.day != day) return (day - k.day);
 else return 0;
}

bool Date:: operator <(Date &k) {
 if (equal(k) < 0) return true;
 else return false;
}
bool Date:: operator <=(Date &k) {
 if (equal(k) <= 0) return true;
 else return false;
}
bool Date:: operator ==(Date &k) {
 if (equal(k) == 0) return true;
 else return false;
}
bool Date:: operator !=(Date &k) {
 if (equal(k) != 0) return true;
 else return false;
}
bool Date:: operator >(Date &k) {
 if (equal(k) > 0) return true;
 else return false;
}
bool Date:: operator >=(Date &k) {
 if (equal(k) >= 0) return true;
 else return false;
}


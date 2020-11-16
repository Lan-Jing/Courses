// Problem#: 17892
// Submission#: 4644042
// The source code is licensed under Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License
// URI: http://creativecommons.org/licenses/by-nc-sa/3.0/
// All Copyright reserved by Informatic Lab of Sun Yat-sen University
#include <iostream>
using namespace std;
int m[12]= {31,28,31,30,31,30,31,31,30,31,30,31};
class Date {
 public:
  Date(int y=0, int m=1, int d=1) {
   year = y;
   month = m;
   day = d;
  };
  static bool leapyear(int year);
  int getYear() const;
  int getMonth() const;
  int getDay() const;
  Date& operator ++();
  Date& operator --();
  Date operator ++(int);
  Date operator --(int);
  //friend ostream &operator <<(ostream &out ,const Date& a);
 private:
  int year,month,day;
};
// ostream &operator <<(ostream &out ,const Date& a)
//{
//  out<<a.year<<" "<<a.month<<" "<<a.day<<endl;
//  return out;
//}
int Date::getYear() const {
 return year;
}
int Date::getMonth() const {
 return month;
}
int Date::getDay() const {
 return day;
}
bool Date::leapyear(int year) {
 if(year%4==0&&year%100!=0||year%400==0)
  return true;
 else
  return false;
}
Date Date::operator --(int) {
 Date a;
 a = *this;
 day--;
 if(day==0&&month!=1) {
  day = m[month-2];
  if(month==3&&leapyear(year))
   day++;
  month--;
 } else if(day==0&&month==1) {
  day = 31;
  month = 12;
  year--;
 }
 return a;
}
Date Date::operator ++(int) {
 Date a;
 a = *this;
 day++;
 if(month!=2) {
  if(day>m[month-1]) {
   day-=m[month-1];
   month++;
  } else if(day>(m[month-1]-1))
   if(month>12) {
    month=1;
    year++;
   }
 } else {
  if(leapyear(year)) {
   if(day>29) {
    month = 3;
    day = 1;
   }
  } else if(!leapyear(year)) {
   if(day>28) {
    month = 3;
    day = 1;
   }
  }
 }
 if(month==13) {
  month =1;
  day = 1;
  year++;
 }
 return a;
}
Date& Date::operator --() {
 day--;
 if(day==0&&month!=1) {
  day = m[month-2];
  if(month==3&&leapyear(year))
   day++;
  month--;
 } else if(day==0&&month==1) {
  day = 31;
  month = 12;
  year--;
 }
 return *this;
}
Date& Date::operator ++() {
 day++;
 if(month!=2) {
  if(day>m[month-1]) {
   day-=m[month-1];
   month++;
  } else if(day>(m[month-1]-1))
   if(month>12) {
    month=1;
    year++;
   }
 } else {
  if(leapyear(year)) {
   if(day>29) {
    month = 3;
    day = 1;
   }
  } else if(!leapyear(year)) {
   if(day>28) {
    month = 3;
    day = 1;
   }
  }
 }
 if(month==13) {
  month =1;
  day = 1;
  year++;
 }
 return *this;
}


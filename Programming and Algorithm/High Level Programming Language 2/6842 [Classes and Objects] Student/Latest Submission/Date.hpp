#ifndef DATE_H
#define DATE_H

#include<iostream>
using namespace std;
#include<cstring>
#include<algorithm>

class Date {
  private:
    int _year;
    int _month;
    int _day;
  public:
    Date(int y, int m, int d);
    Date(){ this->_year = this->_month = this->_day = 0; }
    Date(string dateString); // the format of dateString is like "2017-5-7"
    int getYear() const;
    void setYear(int y);
    int getMonth() const;
    void setMonth(int m);
    int getDay() const;
    void setDay(int d);
    bool operator==(Date& other) const ;
    bool operator<(Date& other) const ;
    bool operator>(Date& other) const ;
    std::string toString() const; // return a string like "year-month-day"
};
Date::Date(int y,int m,int d){
    this->_year = y;
    this->_month = m;
    this->_day = d;
}
Date::Date(string dateString){
    int year = 0,month = 0,day = 0;
    int length = dateString.length();
    int count = 0;
    for(int i = 0;i < length;i++){
        if(dateString[i] != '-'){
            if(!count) year = year*10 + (dateString[i]-'0');
            else if(count == 1) month = month*10 + (dateString[i]-'0');
            else day = day*10 + (dateString[i]-'0');
        }else count++;
    }
    this->_year = year;
    this->_month = month;
    this->_day = day;
}
int Date::getYear() const { return this->_year; }
int Date::getMonth() const { return this->_month; }
int Date::getDay() const { return this->_day; }
void Date::setYear(int y){ this->_year = y; }
void Date::setMonth(int m){ this->_month = m; }
void Date::setDay(int d){ this->_day = d; }
bool Date::operator == (Date& other) const { 
    if(other.getDay() != this->_day ||
       other.getMonth() != this->_month ||
       other.getYear() != this->_year) return false;
    else return true;
}
bool Date::operator < (Date& other) const {
    if(this->_year < other.getYear()) return true;
    else if(this->_year > other.getYear()) return false;
    else if(this->_month < other.getMonth()) return true;
    else if(this->_month > other.getMonth()) return false;
    else if(this->_day < other.getDay()) return true;
    else if(this->_day > other.getDay()) return false;
    return false;
}
bool Date::operator > (Date& other) const {
    if(this->_year > other.getYear()) return true;
    else if(this->_year < other.getYear()) return false;
    else if(this->_month > other.getMonth()) return true;
    else if(this->_month < other.getMonth()) return false;
    else if(this->_day > other.getDay()) return true;
    else if(this->_day < other.getDay()) return false;
    return false;
}
string Date::toString() const {
    string result;
    int day = this->_day;
    int month = this->_month;
    int year = this->_year;
    while(day){
        result += day % 10 + '0';
        day /= 10;
    }
    result += '-';
    while(month){
        result += month % 10 + '0';
        month /= 10;
    }
    result += '-';
    while(year){
        result += year % 10 + '0';
        year /= 10;
    }
    reverse(result.begin(),result.end());
    return result;
}

#endif
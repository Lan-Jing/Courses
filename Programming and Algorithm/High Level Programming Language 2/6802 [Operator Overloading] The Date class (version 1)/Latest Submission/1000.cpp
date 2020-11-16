#include<iostream>
using namespace std;
#include<cstring>

class Date{
    public:
        Date(int y = 0,int m = 1, int d = 1){
            this->year = y;
            this->month = m;
            this->day = d;
        };
        Date(const Date& other){
            this->year = other.getYear();
            this->month = other.getMonth();
            this->day = other.getDay();
        }
        static bool leapyear(int year){
            return ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0));
        };
        int getYear() const { return this->year; }
        int getMonth() const { return this->month; }
        int getDay() const { return this->day; }
        bool operator == (const Date& other) const {
            if((this->year != other.getYear()) || 
               (this->month != other.getMonth()) ||
               (this->day != other.getDay()))
                return false;
            return true;
        }
        bool operator != (const Date& other) const { return !((*this) == other); }
        bool operator < (const Date& other) const {
            int oyear = other.getYear(), omonth = other.getMonth(), oday = other.getDay();
            if(this->year < oyear) return true;
            else if(this->year > oyear) return false;
            else if(this->month < omonth) return true;
            else if(this->month > omonth) return false;
            else if(this->day < oday) return true;
            else if(this->day > oday) return false;
            else return false;
        }
        bool operator > (const Date& other) const {
            return (other < (*this));
        }
        bool operator <= (const Date& other) const {
            return ((*this) == other || (*this) < other);
        }
        bool operator >= (const Date& other) const {
            return ((*this) == other || (*this) > other);
        }
    
    private:
        int year;
        int month;
        int day;
};
#include<iostream>
using namespace std;
#include<cstring>

class Date {
    public:
        Date(int y = 0,int m = 1,int d = 1){
            this->year = y;
            this->month = m;
            this->day = d;
        };
        static bool leapyear(int year){
            if((year % 4 == 0 && year %100) || (year % 400 == 0)) return true;
            return false;
        }
        static int dayPerMonth(int month,int year){
            if((month <= 7 && month % 2) || (month >= 8 && month % 2 == 0)) return 31;
            else if(month == 2){
                if(leapyear(year)) return 29;
                else return 28;
            }else return 30;
        }
        int getYear() const { return this->year; }
        int getMonth() const { return this->month; }
        int getDay() const { return this->day; }
        void increment(){
            if(this->day < dayPerMonth(this->month,this->year)) this->day++;
            else{
                this->day = 1;
                if(this->month == 12) this->month = 1,this->year++;
                else this->month++;
            }
        }
        void decrement(){
            if(this->day > 1) this->day--;
            else{
                if(this->month == 1) this->month = 12,this->year--;
                else this->month--;
                this->day = dayPerMonth(this->month,this->year);
            }
        }
        Date operator + (int num){
            Date result = (*this);
            if(num > 0) for(int i = 1;i <= num;i++) result.increment();
            else for(int i = 1;i <= -num;i++) result.decrement();
            return result;
        }
        Date operator - (int num){
            Date result = (*this);
            if(num > 0) for(int i = 1;i <= num;i++) result.decrement();
            else for(int i = 1;i <= -num;i++) result.increment();
            return result;
        }
        Date operator += (int num){
            (*this) = (*this) + num;
            return *this;
        }
        Date operator -= (int num){
            (*this) = (*this) - num;
            return *this;
        }        
        Date& operator ++ (){
            this->increment();
            return (*this);
        }
        Date& operator -- (){
            this->decrement();
            return (*this);
        }
        Date operator ++ (int){
            Date result = *this;
            this->increment();
            return result;
        }
        Date operator -- (int){
            Date result = *this;
            this->decrement();
            return result;
        }
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
        int& operator [] (const string str) {
            if(str == "year") return this->year;
            if(str == "month") return this->month;
            if(str == "day") return this->day;
        }
    private:
        int year,month,day;
};
#include<iostream>
using namespace std;
#include<cstring>

class Date {
    public:
        Date(int y = 0, int m = 1, int d = 1);
        static bool leapyear(int year);
        static int DayPerMonth(int month,int year);
        int getYear() const ;
        int getMonth() const ;
        int getDay() const ;
        void increment();
        void decrement();
        Date& operator ++ ();
        Date operator  ++ (int);
        Date& operator -- ();
        Date operator  -- (int);

        friend ostream& operator << (ostream&, const Date& );

    private:
        int year,month,day;
};
Date::Date(int y,int m,int d){
    this->year = y;
    this->month = m;
    this->day = d;
}
int Date::getYear() const {
    return this->year;
}
int Date::getMonth() const {
    return this->month;
}
int Date::getDay() const {
    return this->day;
}
bool leapyear(int year){
    if((year % 4 == 0 && year % 100) || (year % 400 == 0)) return true;
    return false;
}
int dayPerMonth(int month,int year){
    if(month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)
        return 31;
    else if(month == 2){
        if(leapyear(year)) return 29;
        else return 28;
    }else return 30;
}
void Date::increment(){
    if(this->day < dayPerMonth(this->month,this->year)){
        this->day++;
    }else{
        this->day = 1;
        if(this->month < 12) this->month++;
        else this->month = 1,this->year++;
    }
}
void Date::decrement(){
    if(this->day > 1){
        this->day--;
    }else{
        if(this->month == 1) this->month = 12, this->year--;
        else this->month--;
        this->day = dayPerMonth(this->month,this->year);
    }
}
Date& Date::operator ++ (){
    this->increment();
    return (*this);
}
Date& Date::operator -- (){
    this->decrement();
    return (*this);
}
Date Date::operator ++ (int){
    Date result = *this;
    this->increment();
    return result;
}
Date Date::operator -- (int){
    Date result = *this;
    this->decrement();
    return result;
}
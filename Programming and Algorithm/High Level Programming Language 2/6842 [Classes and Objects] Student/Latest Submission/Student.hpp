#include<iostream>
using namespace std;
#include "Date.hpp"
#include "Person.hpp"

class Student : public Person {
    public:
        Student(string name,int age,Date date);
        void set(int age);
        void set(string name);
        void set(Date d);
        void sayHi() const ;
    private:
        Date graduateDate;
};
void Student::sayHi() const {
    this->Person::sayHi();
    Date temp_date(2017,5,7);
    if(this->graduateDate > temp_date)
        cout<<"I will graduate on "<<this->graduateDate.toString()<<"."<<endl;
    else if(this->graduateDate < temp_date)
        cout<<"I have graduated on "<<this->graduateDate.toString()<<"."<<endl;
    else cout<<"I graduated today!"<<endl;
}
Student::Student(string name,int age,Date date) : Person(name,age) {
    this->graduateDate.setDay(date.getDay());
    this->graduateDate.setMonth(date.getMonth());
    this->graduateDate.setYear(date.getYear());
    this->Person::set(name);
    this->Person::set(age);
}
void Student::set(Date date){
    this->graduateDate.setDay(date.getDay());
    this->graduateDate.setMonth(date.getMonth());
    this->graduateDate.setYear(date.getYear());
}
void Student::set(int age){
    this->Person::set(age);
}
void Student::set(string name){
    this->Person::set(name);
}
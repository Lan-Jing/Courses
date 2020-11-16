#include<iostream>
using namespace std;
#include "Person.h"

Date::Date(int newYear = 0, int newMonth = 0, int newDay = 0){
    setYear(newYear);
    setMonth(newMonth);
    setDay(newDay);
}
int Date::getYear(){ return year; }
int Date::getMonth(){ return month; }
int Date::getDay(){ return day; }
void Date::setYear(int newYear = 0){ year = newYear; }
void Date::setMonth(int newMonth = 0){ month = newMonth; }
void Date::setDay(int newDay = 0){ day = newDay; }

Person::Person(int _id,int year,int month,int day){
    id = _id;
    birthDate = new Date(year,month,day);
}
Person::Person(Person& other){
    //carefully allocate new space before writing data into a pointer.
    birthDate = new Date(other.getBirthDate()->getYear(),other.getBirthDate()->getMonth(),other.getBirthDate()->getDay());
    id = other.getId();
}
Person::~Person(){
    delete birthDate;
}
int Person::getId(){
    return id;
}
Date* Person::getBirthDate(){
    return birthDate;
}
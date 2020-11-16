#ifndef PERSON_H_
#define PERSON_H_

#include <iostream>
using namespace std;

class Date {
 public:
  Date(int newYear, int newMonth, int newDay);
  int getYear();
  void setYear(int newYear);
  int getMonth();
  void setMonth(int);
  int getDay();
  void setDay(int);

 private:
  int year;
  int month;
  int day;
};

class Person {
 public:
  Person(int _id, int year, int month, int day);
  Person(Person &); // copy constructor
  ~Person();
  int getId();
  Date * getBirthDate();

 private:
  int id;
  Date *birthDate;
};


#endif

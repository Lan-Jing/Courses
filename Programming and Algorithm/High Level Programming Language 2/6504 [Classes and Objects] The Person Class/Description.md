# [Classes and Objects] The Person Class

## Description
```
Implement the Date class and Person class using the following headers:
class Date
{
    public:
        Date(int newYear,int newMonth,int newDay);
        int getYear();
        void setYear(int newYear);
        int getMonth();
        int getDay();
        void setMonth(int Month);
        void setDay(int Day);

  private:
        int year;
        int month;
        int day;
};

class Person
{
public:
  Person(int id, int year, int month, int day);
  Person(const Person &); // copy constructor
  ~Person();
  int getId();
  Date * getBirthDate();
  static int getNumberOfObjects(); //return the number of Person objects 
 
private:
  int id;
  Date *birthDate; 
  static int numberOfObjects; //count the number of Person objects
};
```
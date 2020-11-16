# [Operator Overloading] The Date class (version 1)

# Description
```
Implement the operator <, <=, ==, !=, >, >= with the class Date
class Date
{
public:
  Date(int y=0, int m=1, int d=1);  
  static bool leapyear(int year);
  int getYear() const;
  int getMonth() const;
  int getDay() const;

  // add any member you need here  
};

You implementation should enable the usage like this:
void f()
{
  Date date1, date2(2003,1,1);
  Date date3 = Date(2007,2,28);
  date3 = date1;

  cout << "year 1996 is leap year? " << Date::leapyear(1996) << endl;
  cout << "year 1200 is leap year? " << Date::leapyear(1200) << endl;
  cout << "year 1300 is leap year? " << Date::leapyear(1300) << endl;
  cout << "year 1999 is leap year? " << Date::leapyear(1999) << endl;

  cout <<  "(date1==date3)? " << (date1==date3) << endl;
  cout <<  "(date1!=date3)? " << (date1!=date3) << endl;
  cout <<  "(date1==date2)? " << (date1==date2) << endl;
  cout <<  "(date1!=date2)? " << (date1!=date2) << endl;

  cout <<  "(date1<date1)? " << (date1<date1) << endl;
  cout <<  "(date1<=date1)? " << (date1<=date1) << endl;
  cout <<  "(date1<date2)? " << (date1<date2) << endl;
  cout <<  "(date1<=date2)? " << (date1<=date2) << endl;
 
  cout <<  "(date1>date1)? " << (date1>date1) << endl;
  cout <<  "(date1>=date1)? " << (date1>=date1) << endl;
  cout <<  "(date1>date2)? " << (date1>date2) << endl;
  cout <<  "(date1>=date2)? " << (date1>=date2) << endl;
}
The output of f() should be:
year 1996 is leap year? 1
year 1200 is leap year? 1
year 1300 is leap year? 0
year 1999 is leap year? 0
(date1==date3)? 1
(date1!=date3)? 0
(date1==date2)? 0
(date1!=date2)? 1
(date1<date1)? 0
(date1<=date1)? 1
(date1<date2)? 1
(date1<=date2)? 1
(date1>date1)? 0
(date1>=date1)? 1
(date1>date2)? 0
(date1>=date2)? 0
```

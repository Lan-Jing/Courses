# [Operator Overloading] The Date class (version 3)

# Description
```
Implement the operator +=, -=, +, -, in the class Date
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
  Date date = d;

  cout << "date = " << date << endl;
  cout << "date+1 = " << date+1 << endl;
  cout << "date-1 = " << date-1 << endl;

  date+=366;
  cout << "date = " << date << endl;
  date-=365;
  cout << "date = " << date << endl;
  date-=-365;
  cout << "date = " << date << endl;
  date+=-366;
  cout << "date = " << date << endl;
      
  cout << endl;
}


The output of f() should be:

date = 2004-2-28
date+1 = 2004-2-29
date-1 = 2004-2-27
date = 2005-2-28
date = 2004-2-29
date = 2005-2-28
date = 2004-2-28

提交时不需要提交operator << 重载。
```

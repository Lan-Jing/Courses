# [Operator Overloading]The Date class (version 2)

# Description
```
Implement the operator ++(prefix), --(prefix), ++(postfix), --(postfix), in the class Date
class Date
{
public:
  Date(int y=0, int m=1, int d=1); 
  static bool leapyear(int year);
  int getYear() const;
  int getMonth() const;
  int getDay() const;

  friend ostream& operator<<(ostream&, const Date&);  
  // add any member you need here 
};

You implementation should enable the usage like this:
void f()
{
  Date date(2004,2,28);
  cout << "date = " << date << endl;
  cout << "++date = " << ++date << endl;
  cout << "--date = " << --date << endl;
  cout << "date++ = " << date++ << endl;
  cout << "date-- = " << date-- << endl;
  cout << "date = " << date << endl;
}

The output of f() should be:

date = 2004-2-28
++date = 2004-2-29
--date = 2004-2-28
date++ = 2004-2-28
date-- = 2004-2-29
date = 2004-2-28

提交时不需要提交operator << 重载。
```

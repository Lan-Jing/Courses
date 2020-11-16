#include <cstring>
#include <iostream>
using namespace std;

class TypeA
{
public:
  class SubType
  {
  public:
    string toString() {return "subType in TypeA";};
  };
};

class TypeB
{
public:
  class SubType
  {
  public:
    string toString() {return "subType in TypeB";};
  };
};

template <typename T>
class MyClass {
public:
    typename T::SubType subtypeobj;
};
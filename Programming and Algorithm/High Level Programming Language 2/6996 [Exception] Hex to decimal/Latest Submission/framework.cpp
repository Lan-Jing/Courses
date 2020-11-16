#include "source.h"
#include <iostream>
#include <cstring>
using namespace std;

void f(const char* const hexString)
{
  try
  {
    cout << parseHex(hexString) << endl;
  }
  catch(runtime_error ex)
  {
    cout << ex.what() << endl;
  }
}

int main()
{
  f("10");
  f("ABC");
  f("A5");
  f("FAA");
  f("10A");
  f("9G3");
  f("*H");
  f("99A");
  return 0;
}

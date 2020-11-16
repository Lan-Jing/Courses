#include <iostream>
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
using namespace std;
#include"1001.cpp"
int Int::num=0;
void f()
{
 int i1=1, i2=2;
 cout << "num " << i1 << " is odd? " << Int::isodd(1) << endl; 
 cout << "num " << i2 << " is odd? " << Int::isodd(2) << endl; 
    Int ii1; 
    { Int ii1(i1); Int ii2; }
    Int ii2(i2);
}

int main()
{
    f();
 return 0;
}

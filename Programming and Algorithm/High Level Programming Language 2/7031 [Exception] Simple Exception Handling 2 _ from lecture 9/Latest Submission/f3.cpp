#include <iostream>
#include "f3.hpp"
using namespace std;

void f3(int x)                        //EX9_2.cpp
{
    switch (x)
    {
    case 1:
        throw 3.4;        // 抛出double型异常
    case 2:
        throw 2.5f;           // 抛出float型异常
    case 3:
        throw 1;          // 抛出int型异常
    }
    cout << "End of f3" << endl;
}
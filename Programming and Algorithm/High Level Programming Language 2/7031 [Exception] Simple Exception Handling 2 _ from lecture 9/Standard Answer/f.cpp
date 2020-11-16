#include <iostream>
#include "f.hpp"
#include "f3.hpp"
using namespace std;


void f2(int x)
{
    try
    {
        f3(x);
    }
    catch (int)  	//int型异常的处理代码
    {
        cout << "An int exception occurred!--from f2" << endl;
    }
    catch (float)   	//float型异常的处理代码
    {
        cout << "A float exception occurred!--from f2" << endl;
    }
    cout << "End of f2" << endl;
}

void f1(int x)
{
    try
    {
        f2(x);
    }
    catch (int)    	            // int型异常的处理代码
    {
        cout << "An int exception occurred!--from f1" << endl;
    }
    catch (float)    	            // float型异常的处理代码
    {
        cout << "A float exception occurred!--from f1" << endl;
    }
    catch (double)   	            // double型异常的处理代码
    {
        cout << "A double exception occurred!--from f1" << endl;
    }
    cout << "End of f1" << endl;
}
#include <iostream>
#include "area.hpp"
using namespace std;

int main()
{
    double a, b, c;	//三角形三边长
    for (int i = 0; i < 3; i++)
    {
        cin >> a >> b >> c;
        try
        {
            double s = area(a, b, c);	//尝试计算三角形面积
            cout << "Area: " << s << endl;
        }
        catch (exception &e)
        {
            cout << "Error: " << e.what() << endl;
        }
    }
    return 0;
}
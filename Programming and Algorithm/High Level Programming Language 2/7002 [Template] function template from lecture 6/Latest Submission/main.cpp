#include "swap.hpp"
#include <string>
#include <iostream>
int main()
{
    std::string s1("rabbit"), s2("bear");
    int iv1 = 3, iv2 = 5;
    double dv1 = 2.8, dv2 = 8.5;

    // 调用函数模板的实例swap(string&, string&)
    myNamespace::swap(s1, s2);
    std::cout << "s1: " << s1 << ", s2: " << s2 << std::endl;

    // 调用函数模板的实例swap(int&, int&)
    myNamespace::swap(iv1, iv2);
    std::cout << "iv1: " << iv1 << ", iv2: " << iv2 << std::endl;

    // 调用swap的实例swap(double&, double&)
    myNamespace::swap(dv1, dv2);
    std::cout << "dv1: " << dv1 << ", dv2 " << dv2 << std::endl;
    return 0;
}

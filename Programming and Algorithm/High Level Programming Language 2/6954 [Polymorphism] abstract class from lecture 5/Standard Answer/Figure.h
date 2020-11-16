#ifndef FIGURE_H
#define FIGURE_H
const double PI = 3.14159; // 圆周率常量

class FIGURE
{
public:
    void set_size(double x, double y = 0);
    virtual double get_area() = 0; // get_area()被声明为纯虚函数
protected:
    double x_size, y_size;
};

#endif

#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Figure.h"

class TRIANGLE : public FIGURE
{
public:
    virtual double get_area();
};

double TRIANGLE::get_area()
{
    return (x_size * y_size / 2); // 三角形面积 = 底×高÷2
}

#endif
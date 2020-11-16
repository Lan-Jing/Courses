#ifndef CIRCLE_H
#define CIRCLE_H

#include "Figure.h"

class CIRCLE : public FIGURE
{
public:
    virtual double get_area();
};

double CIRCLE::get_area()
{
    return (PI * x_size * x_size); // 圆面积 = p×半径×半径
}

#endif

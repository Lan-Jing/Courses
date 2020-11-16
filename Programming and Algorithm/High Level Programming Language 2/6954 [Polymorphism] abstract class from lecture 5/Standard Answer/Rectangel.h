#ifndef RETANGLE_H
#define RETANGLE_H

#include "Figure.h"

class RECTANGLE : public FIGURE
{
public:
    virtual double get_area();
};

double RECTANGLE::get_area()
{
    return (x_size * y_size); // 矩形面积 = 长×宽
}

#endif

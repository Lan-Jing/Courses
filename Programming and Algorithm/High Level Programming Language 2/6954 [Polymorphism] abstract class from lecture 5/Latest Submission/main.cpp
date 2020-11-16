#include "Figure.h"
#include "Triangle.h"
#include "Rectangel.h"
#include "Circle.h"
#include <iostream>
using namespace std;

int main()
{
       TRIANGLE triangle;	 
       RECTANGLE rectangle;		
       CIRCLE circle;			

        // 处理三角形
        triangle.set_size(15, 8);	// 设置三角形的底和高
        cout << "Area of triangle is " << triangle.get_area() << "\n";
	
        // 处理矩形
        rectangle.set_size(15, 8);	// 设置矩形的长和宽
        cout << "Area of rectangle is " << rectangle.get_area() << "\n";
	
        // 处理圆
        circle.set_size(15);		// 设置圆的半径
        cout << "Area of circle is " << circle.get_area() << "\n";
	
       return 0;
}
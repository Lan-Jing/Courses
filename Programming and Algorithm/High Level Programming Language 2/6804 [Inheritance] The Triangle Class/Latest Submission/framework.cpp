#include <iostream>
#include <string>
#include "source.hpp "
using namespace std;
int main()
{
  Triangle shape1;
  cout << shape1.getSide1() << endl;
  cout << shape1.getSide2() << endl;
  cout << shape1.getSide3() << endl;  
  cout << shape1.getArea() << endl;
  cout << shape1.getPerimeter() << endl;
  cout << shape1.getColor() << endl;
  cout << shape1.isFilled() << endl;

  Triangle shape(1, 1.5, 1);
  cout << shape.getColor() << endl;
  cout << shape.isFilled() << endl;

  shape.setColor("yellow");
  shape.setFilled(true);

  cout << shape.getSide1() << endl;
  cout << shape.getSide2() << endl;
  cout << shape.getSide3() << endl;  
  cout << shape.getArea() << endl;
  cout << shape.getPerimeter() << endl;
  cout << shape.getColor() << endl;
  cout << shape.isFilled() << endl;

  return 0;
}

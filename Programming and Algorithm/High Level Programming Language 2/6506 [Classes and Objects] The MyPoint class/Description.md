# [Classes and Objects] The MyPoint class

## Description
```
Design a class named MyPoint to represent a point with x and y-coordinates. The class contains:
Two double data fields x and y that represent the coordinates.
A no-arg constructor that creats a point(0,0).
A counstructor that constructs a point with specified coordinates.
Two get functions for data fields x and y, respectively.
A function distance that returns the distance from this point to another point of the MyPoint type.
class MyPoint
{
  private:
    double x, y;
  public:
    MyPoint();
    MyPoint(double x, double y);
    double getX();
    double getY();
    double distance(MyPoint &point);
};
```
## Input
```
1 5 
3 3
1 1
0 0
```
## Output
```



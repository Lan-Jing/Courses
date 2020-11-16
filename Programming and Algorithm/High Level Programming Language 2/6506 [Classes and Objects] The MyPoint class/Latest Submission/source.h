#include<iostream>
#include<cmath>
using namespace std;

class MyPoint{
    private:
        double x,y;
    public:
        MyPoint();
        MyPoint(double x,double y);
        double getX();
        double getY();
        double distance(MyPoint &point);
};

MyPoint::MyPoint(){
    x = y = 0;
}
MyPoint::MyPoint(double xx, double yy){
    x = xx,y = yy;
}
double MyPoint::getX(){
    return x;
}
double MyPoint::getY(){
    return y;
}
double MyPoint::distance(MyPoint &point){
    return sqrt((x - point.getX())*(x - point.getX()) + (y - point.getY())*(y - point.getY()));
}
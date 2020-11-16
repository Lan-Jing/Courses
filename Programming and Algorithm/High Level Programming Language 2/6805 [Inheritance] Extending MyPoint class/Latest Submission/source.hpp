#include<iostream>
using namespace std;
#include<cstring>
#include<cmath>

class MyPoint{
    public:
        MyPoint(){ this->x = this->y = 0;}
        MyPoint(double x,double y){ this->x = x, this->y = y; }
        double getX() const { return this->x; }
        double getY() const { return this->y; }
        double distance(const MyPoint& point){
            return sqrt((this->x-point.getX())*(this->x-point.getY()) +
                        (this->y-point.getY())*(this->y-point.getY()));
        }
    private:
        double x,y;
};

class ThreeDPoint : public MyPoint {
    public:
        ThreeDPoint(){ this->z = 0; }
        ThreeDPoint(double x, double y, double z) :MyPoint(x,y) {
            this->z = z;
        }
        double getZ() const { return this->z; }
        double distance(const ThreeDPoint& other){
            double x = this->getX(),y = this->getY() ,z = this->getZ();
            double xx = other.getX(),yy = other.getY() ,zz = other.getZ();
            return sqrt((x-xx)*(x-xx) + (y-yy)*(y-yy) + (z-zz)*(z-zz));
        }
    private:
        double z; 
};
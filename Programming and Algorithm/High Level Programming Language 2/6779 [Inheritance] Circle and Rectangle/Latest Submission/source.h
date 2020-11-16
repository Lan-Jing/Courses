#include<iostream>
using namespace std;
#include<cstring>
#include<cmath>

class Point{
    public:
        Point(){ this->x = this->y = 0; }
        Point(double xv,double yv){ this->x=xv,this->y=yv; }
        Point(Point& pt){ this->x=pt.getx(), this->y=pt.gety(); }
        double getx(){ return this->x; }
        double gety(){ return this->y; }
    private:
        double x,y;
};
class Circle: public Point{
    public:
        Circle() :Point() { this->r = 0; }
        Circle(double xv,double yv,double rv) :Point(xv,yv) {
            this->r = rv;
        }
        Circle(Circle& other) :Point(other.getx(),other.gety()) { this->r = other.getr(); }
        double getr(){ return this->r; }
        int position(Point p){
            double dx = p.getx()-this->getx();
            double dy = p.gety()-this->gety();
            if(dx*dx + dy*dy == r*r) return 0;
            else if(dx*dx + dy*dy > r*r) return 1;
            else return -1;
        }
    private:
        double r; 
};
class Rectangle: public Point{
    public:
        Rectangle(){ this->length = this->width = 0; }
        Rectangle(double xv,double yv,double l,double w) :Point(xv,yv) {
            this->length = l, this->width = w;
        }
        Rectangle(Rectangle& other) :Point(other.getx(),other.gety()) {
            this->length = other.getl(),this->width = other.getw();
        }
        double getl(){ return this->length; }
        double getw(){ return this->width; }
        int position(Point p){
            int x = p.getx(),y = p.gety();
            if(this->getx() < x && x < this->getx()+this->length && this->gety() < y && y < this->gety()+this->width) return -1;
            else if(this->getx() > x || this->getx()+this->length < x || this->gety() > y || this->gety()+this->width < y) return 1;
            else return 0;
        }
    private:
        double length,width;
};
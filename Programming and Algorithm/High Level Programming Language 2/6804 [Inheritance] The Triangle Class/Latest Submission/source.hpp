#include<iostream>
using namespace std;
#include<cmath>

class GeometricObject{
    public:
        GeometricObject(string color, bool filled){
            this->color = color;
            this->filled = filled;
        }
        string getColor() { return this->color; }
        void setColor(string color) { this->color = color; }
        bool isFilled() { return this->filled; }
        void setFilled(bool filled) { this->filled = filled; }
        string toString(){
            return "Geometric object color" + color + "filled" + ((filled) ? "true" : "false");
        }
    private:
        string color;
        bool filled;
};

class Triangle : public GeometricObject{
    public:
        Triangle() :GeometricObject("blue",true) { 
            this->side1 = this->side2 = this->side3 = 1;
        }
        Triangle(double s1, double s2, double s3) :GeometricObject("blue",true) {
            this->side1 = s1;
            this->side2 = s2;
            this->side3 = s3;
        }
        double getSide1() const { return this->side1; }
        double getSide2() const { return this->side2; }
        double getSide3() const { return this->side3; }
        double getPerimeter() const { return this->side1 + this->side2 + this->side3; }
        double getArea() const {
            double hp = this->getPerimeter()/2;
            return sqrt(hp*(hp-this->side1)*(hp-this->side2)*(hp-this->side3));
        }

    private:
        double side1,side2,side3;
};

#include<iostream>
using namespace std;
#include<cstring>

class Point{
    protected:
        int x,y;
    public:
        Point(int x,int y){ this->x=x,this->y=y;}
        void Move(int x,int y){ this->x+=x,this->y+=y;}
        int Getx(){ return this->x; }
        int Gety(){ return this->y; }
};
class Rectangle: public Point {
    private:
        int length;
        int width;
    public:
        Rectangle(int init_x,int init_y,int initl,int initw): Point(init_x,init_y){
            this->length = initl;
            this->width = initw;
        }// 用初始化列表来指定调用父类的构造函数
        int Getlength(){ return this->length; }
        int Getwidth(){ return this->width; }
        int Getarea(){ return (this->width)*(this->length); }
};
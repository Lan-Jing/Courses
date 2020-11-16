#include<iostream>
using namespace std;
#include "Figure.h"

class TRIANGLE : public FIGURE {
public:
	void set_size(double bottom,double height){
		this->bottom = bottom;
		this->height = height;
	}
	double get_area(){ return this->bottom*this->height*0.5; }
private:
	double bottom,height;
};

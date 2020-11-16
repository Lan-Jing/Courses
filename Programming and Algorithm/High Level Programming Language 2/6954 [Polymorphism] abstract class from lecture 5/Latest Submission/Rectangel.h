#include<iostream>
using namespace std;
#include "Figure.h"

class RECTANGLE : public FIGURE {
public:
	void set_size(double length, double width){
		this->length = length;
		this->width = width;
	};
	double get_area(){ return this->length*this->width; }
private:
	double length,width;
};
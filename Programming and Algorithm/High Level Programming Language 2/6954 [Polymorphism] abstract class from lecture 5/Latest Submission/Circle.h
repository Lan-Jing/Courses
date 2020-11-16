#include<iostream>
using namespace std;
#include "Figure.h"

class CIRCLE : public FIGURE {
public:
	void set_size(double r){ this->radium = r; }
	double get_area(){ return this->radium*this->radium*PI; }
private:
	double radium;
};
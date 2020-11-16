#include<iostream>
using namespace std;
#include"area.hpp"
#include<cmath>

double area(double a, double b, double c) throw (invalid_argument) {
	if(a <= 0 || b <= 0 || c <= 0)
		throw invalid_argument("the side length should be positive");
	else if(a + b < c || b + a < c || a + c < b)
		throw invalid_argument("the side length should fit the triangle inequation");
	else{
		double halfPerimeter = (a+b+c)/2;
		double S = sqrt(halfPerimeter*(halfPerimeter-a)*(halfPerimeter-b)*(halfPerimeter-c));
		return S;
	}
}
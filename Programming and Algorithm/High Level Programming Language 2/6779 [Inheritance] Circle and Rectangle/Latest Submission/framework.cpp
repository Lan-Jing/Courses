#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;
#include "source.h"
const double PI=3.14;
int main(){
	//freopen("test02.out", "w", stdout);
	Circle cc1(3,4,5),cc2,cc3(cc1);
	Rectangle rt1(0,0,6,8),rt2,rt3(rt1);
	Point p1(0,0),p2(6,8),p3(3,3),p4(8,4),p5(8,8);
//	cc1.Show();
//	cc2.Show();
//	rt1.Show();
//	rt2.Show();
	cout<<"point p1:";
//	p1.Show();
//	cout<<"矩形rt3:"<<'\t';
//	rt3.Show();
	switch(rt3.position(p1)){
	case 0:cout<<"on-rectangle"<<endl;break;
	case -1:cout<<"inside-rectangle"<<endl;break;
	case 1:cout<<"outside-rectangle"<<endl;break;
	}
//	cout<<"圆cc3:"<<'\t';
//	cc3.Show();
	switch(cc3.position(p1)){
	case 0:cout<<"on-circle"<<endl;break;
	case -1:cout<<"inside-circle"<<endl;break;
	case 1:cout<<"outside-circle"<<endl;break;
	}
	cout<<"point p2:";
//	p2.Show();
//	cout<<"矩形rt3:"<<'\t';
//	rt3.Show();
	switch(rt3.position(p2)){
	case 0:cout<<"on-rectangle"<<endl;break;
	case -1:cout<<"inside-rectangle"<<endl;break;
	case 1:cout<<"outside-rectangle"<<endl;break;
	}
//	cout<<"圆cc3:"<<'\t';
//	cc3.Show();
	switch(cc3.position(p2)){
	case 0:cout<<"on-circle"<<endl;break;
	case -1:cout<<"inside-circle"<<endl;break;
	case 1:cout<<"outside-circle"<<endl;break;
	}
	cout<<"point p3:";
//	p3.Show();
//	cout<<"矩形rt3:"<<'\t';
//	rt3.Show();
	switch(rt3.position(p3)){
	case 0:cout<<"on-rectangle"<<endl;break;
	case -1:cout<<"inside-rectangle"<<endl;break;
	case 1:cout<<"outside-rectangle"<<endl;break;
	}
//	cout<<"圆cc3:"<<'\t';
//	cc3.Show();
	switch(cc3.position(p3)){
	case 0:cout<<"on-circle"<<endl;break;
	case -1:cout<<"inside-circle"<<endl;break;
	case 1:cout<<"outside-circle"<<endl;break;
	}
	cout<<"point p4:";
//	p4.Show();
//	cout<<"矩形rt3:"<<'\t';
//	rt3.Show();
	switch(rt3.position(p4)){
	case 0:cout<<"on-rectangle"<<endl;break;
	case -1:cout<<"inside-rectangle"<<endl;break;
	case 1:cout<<"outside-rectangle"<<endl;break;
	}
//	cout<<"圆cc3:"<<'\t';
//	cc3.Show();
	switch(cc3.position(p4)){
	case 0:cout<<"on-circle"<<endl;break;
	case -1:cout<<"inside-circle"<<endl;break;
	case 1:cout<<"outside-circle"<<endl;break;
	}
	cout<<"point p5:";
//	p5.Show();
//	cout<<"矩形rt3:"<<'\t';
//	rt3.Show();
	switch(rt3.position(p5)){
	case 0:cout<<"on-rectangle"<<endl;break;
	case -1:cout<<"inside-rectangle"<<endl;break;
	case 1:cout<<"outside-rectangle"<<endl;break;
	}
//	cout<<"圆cc3:"<<'\t';
//	cc3.Show();
	switch(cc3.position(p5)){
	case 0:cout<<"on-circle"<<endl;break;
	case -1:cout<<"inside-circle"<<endl;break;
	case 1:cout<<"outside-circle"<<endl;break;
	}
	return 0;
}

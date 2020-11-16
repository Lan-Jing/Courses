#include <iostream>
using namespace std;
#include "f3.hpp"
#include "f.hpp"

void f1(int x){
	try{
		f2(x);
	}catch(int){
		cout<<"An int exception occurred!--from f1"<<endl;
	}catch(float){
		cout<<"A float exception occurred!--from f1"<<endl;
	}catch(double){
		cout<<"A double exception occurred!--from f1"<<endl;
	}
	cout<<"End of f1"<<endl;
}
void f2(int x){
	try{
		f3(x);
	}catch(int){
		cout<<"An int exception occurred!--from f2"<<endl;
	}catch(float){
		cout<<"A float exception occurred!--from f2"<<endl;
	}
	cout<<"End of f2"<<endl;
}

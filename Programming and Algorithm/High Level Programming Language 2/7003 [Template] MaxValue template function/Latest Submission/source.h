#include<iostream>
using namespace std;

template <typename ElementType>
ElementType maxValue(ElementType a, ElementType b){
	return a < b ? b : a;
}

char maxValue(char a, char b){
	char tempa,tempb;
	if('a' <= a && a <= 'z') tempa = a-'a'+'A';
	else tempa = a;
	if('a' <= b && b <= 'z') tempb = b-'a'+'A';
	else tempb = b;
	return tempa < tempb ? b : a;
}
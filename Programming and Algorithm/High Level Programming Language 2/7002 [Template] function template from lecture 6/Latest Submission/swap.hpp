#include<iostream>

namespace myNamespace{
	template <typename ElementType> 
	void swap(ElementType& a, ElementType& b){
		ElementType temp = a;
		a = b;
		b = temp;
	}
}
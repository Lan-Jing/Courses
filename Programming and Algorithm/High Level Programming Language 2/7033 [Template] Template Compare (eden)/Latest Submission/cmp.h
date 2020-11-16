#include<stdio.h>
#include<cstring>
template <typename ElementType>
bool cmp(ElementType a, ElementType b){
	return a == b;
}

bool cmp(char *a, char *b){
	if(strlen(a) != strlen(b)) return false;
	else{
		int length = strlen(a);
		for(int i = 0;i < length;i++)
			if(a[i] != b[i]) return false;
		return true;
	}
}

template <typename ElementType>
bool cmp(ElementType *a, ElementType *b){
	return *a == *b;
}
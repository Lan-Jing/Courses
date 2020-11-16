#ifndef Swap_h
#define Swap_h

void swap(int *a,int *b){
	int temp = *a;*a = *b,*b = temp;
}

#endif
int max(int a,int b){
	return a > b ? a : b;
}
int min(int a,int b){
	return a < b ? a : b;
}

typedef int (*PF)(int a,int b);
PF maxmin(int c){
	if(c == 1) return max;
	else return min;
}
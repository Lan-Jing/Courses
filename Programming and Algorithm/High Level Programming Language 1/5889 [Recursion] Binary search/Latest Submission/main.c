#include<stdio.h>

int n,m;

int binary_serach(int *array,int target,int n){
	int index,l = 0,r = n-1;
	while(l < r){
		int mid = (l+r)/2;
		if(target == array[mid]) return mid;
		if(target > array[mid]) l = mid+1; 
		else r = mid;
	}

	return r;
}

int main(){
	int array[100010];
	scanf("%d",&n);

	int i;
	for(i = 0;i < n;i++) scanf("%d",&array[i]);
	scanf("%d",&m);
	
	while(m--){
		int target;
		scanf("%d",&target);
		printf("%d\n",binary_serach(array,target,n));
	}
	return 0;
}
#include<stdio.h>

int f[40];

int main(){
	f[1] = f[2] = 1;
	int i;
	for(i = 3;i <= 35;i++)
		f[i] = f[i-1] + f[i-2];

	int n;
	scanf("%d",&n);
	printf("%d\n",f[n]);
	return 0;
}
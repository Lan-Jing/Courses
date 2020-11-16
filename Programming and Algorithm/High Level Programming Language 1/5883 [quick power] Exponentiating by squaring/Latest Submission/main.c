#include<stdio.h>

int a,b,p;
int qpow(int a,int b,int p){
	int item = a % p,ans = 1;
	while(b){
		if(b%2) ans = (ans * item)%p;
		item = (item * item)%p;
		b /= 2;
	}
	return ans;
}

int main(){
	scanf("%d %d %d",&a,&b,&p);
	printf("%d\n",qpow(a,b,p));

	return 0;
}
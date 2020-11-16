#include<stdio.h>

int l,r,ans;
int max(int a,int b){
	if(a>b) return a; else return b;
}

int p;
int cal(int x){
	p = 1;
	do{
		if(x % 2) x = 3*x+1;
		else x /= 2;
		p++;
	}while(x != 1);
	return p;
}

int main(){
	scanf("%d %d",&l,&r);
	for(int i = l;i <= r;i++)
		ans = max(ans,cal(i));
	printf("%d\n",ans);
	return 0;
}
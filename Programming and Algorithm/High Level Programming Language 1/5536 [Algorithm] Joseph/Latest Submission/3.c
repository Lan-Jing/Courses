#include<stdio.h>

int m,n;
int a[1010],p,count;

int main(){
	scanf("%d %d",&n,&m);
	count = n; p = m;
	while(count){
		a[p] = 1 , count--;
//		printf("%d ",p);
	
		if(!count){
			printf("%d\n",p); break;
		}
		
		int temp_count = 0;
		
		while(temp_count < m)
			if(a[p%n+1]) p = p%n+1;
			else p = p%n+1 , temp_count++;
	}
	return 0;
}
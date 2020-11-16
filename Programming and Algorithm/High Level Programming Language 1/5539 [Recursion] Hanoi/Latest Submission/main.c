#include<stdio.h>

int n;
void dfs(int n,int b,int a,int c)//½«n¸öÖù×Ó£¬½èÓÃb£¬´ÓaÒÆ¶¯µ½c¡£ 
{	
	if(n == 1){
		printf("%d%d\n",a,c);
		return ;
	}
	dfs(n-1,c,a,b);
	printf("%d%d\n",a,c);
	dfs(n-1,a,b,c);
}
int main(){
	scanf("%d",&n);
	dfs(n,2,1,3);
	return 0;
}
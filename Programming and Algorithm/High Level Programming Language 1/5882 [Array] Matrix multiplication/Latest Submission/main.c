#include<stdio.h>

int n,m,r;
int ma[101][101],mb[101][101],mc[101][101];

void f(){
	int i,j,k;
	for(i = 1;i <= n;i++)
		for(j = 1;j <= r;j++)
			for(k = 1;k <= m;k++)
				mc[i][j] += ma[i][k]*mb[k][j];
}

int main(){
	scanf("%d %d %d ",&n,&m,&r);
	int i,j;

	for(i = 1;i <= n;i++)
		for(j = 1;j <= m;j++)
			scanf("%d",&ma[i][j]);
	for(i = 1;i <= m;i++)
		for(j = 1;j <= r;j++)
			scanf("%d",&mb[i][j]);

	f();

	for(i = 1;i <= n;i++)
		for(j = 1;j <= r;j++)
			if(j < r) printf("%d ",mc[i][j]);
			else printf("%d\n",mc[i][j]);

	return 0;	
}

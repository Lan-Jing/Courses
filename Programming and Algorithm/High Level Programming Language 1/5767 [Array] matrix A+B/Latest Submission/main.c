#include<stdio.h>

int ma[105][105];

int main(){
	int n;
	scanf("%d",&n);
	
	int i,j;
	for(i = 1;i <= n;i++)
		for(j = 1;j <= n;j++)
			scanf("%d",&ma[i][j]);

	int temp;
	for(i = 1;i <= n;i++){
		for(j = 1;j <= n;j++){
			scanf("%d",&temp);
			ma[i][j] += temp; 
			if(j < n) printf("%d ",ma[i][j]);
			else printf("%d\n",ma[i][j]);
		}
	}
	return 0;
}
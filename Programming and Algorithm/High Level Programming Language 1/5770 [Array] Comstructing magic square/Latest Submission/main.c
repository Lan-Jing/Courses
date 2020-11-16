#include<stdio.h>

int matrix[20][20],n;

int main(){
	scanf("%d",&n);

	int now;
	int lasty = 1,lastx = n/2+1;
	matrix[lasty][lastx] = 1;
	for(now = 2;now <= n*n;now++){
		int nowx,nowy;
		if(lasty == 1 && lastx == n)
			nowy = lasty+1,nowx = lastx;
		else{
			if(lasty == 1)
				nowy = n,nowx = lastx + 1;
			else if(lastx == n)
				nowx = 1,nowy = lasty - 1;
			else nowx = lastx+1,nowy = lasty-1;

			if(matrix[nowy][nowx])
				nowy = lasty+1,nowx = lastx;
		}
		matrix[nowy][nowx] = now;
		lasty = nowy,lastx = nowx;
	}

	int i,j;
	for(i = 1;i <= n;i++)
		for(j = 1;j <= n;j++)
			if(j < n) printf("%d ",matrix[i][j]);
			else printf("%d\n",matrix[i][j]);

	return 0;
}
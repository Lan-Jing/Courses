#include<stdio.h>

int n,temp[10],used[10];

void dfs(int step){
	if(step == n+1){
		int i;
		for(i = 1;i <= n;i++) printf("%d",temp[i]);
		printf("\n");
		return ;
	}else{
		int i;
		for(i = 1;i <= n;i++)
			if(!used[i]){
				used[i] = 1;
				temp[step] = i;
				dfs(step+1);
				used[i] = 0;
			}
	}
}

int main(){
	scanf("%d",&n);

	dfs(1);

	return 0;
}
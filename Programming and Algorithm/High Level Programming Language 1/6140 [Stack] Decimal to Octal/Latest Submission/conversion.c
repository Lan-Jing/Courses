#include<stdio.h>
#include<string.h>

int stack[110],top;

int main(){
	int n; scanf("%d",&n);

	int i;
	for(i = 1;i <= n;i++){
		int now; scanf("%d ",&now);

		while(now){
			stack[++top] = now%8;
			now /= 8;
		}

		int ans = 0;
		while(top)
			ans = 10*ans + stack[top--];

		printf("%d\n",ans);

		memset(stack,0,sizeof(stack)); top = 0;
	}
	return 0;
}
#include<stdio.h>
#include<string.h>
#include<math.h>

int n,isnt_prime[500010];
int prime[110],ptr;

void make_prime(){
	isnt_prime[1] = 1;
	int i;
	for(i = 2;i <= sqrt(500010);i++){
		if(isnt_prime[i]) continue;
		prime[++ptr] = i;
		int j;
		for(j = 2;i*j < 500010;j++)
			isnt_prime[i*j] = 1;
	}
//	for(i = 1;i <= 100;i++) printf("%d ",prime[i]);
}

int main(){
	make_prime();

	scanf("%d",&n);	

	int i;
	for(i = 1;i <= n;i++){
		if(i < n) printf("%d ",prime[i]);
		else printf("%d\n",prime[i]);
	}
	return 0;
}
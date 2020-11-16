#include<stdio.h>
#include<math.h>

long long a,b,x1,x2;
int t;
long long gcd(long long a,long long b){
	if(!b) return a;
	else return gcd(b,a%b);
}

int main(){
	scanf("%d",&t);
	while(t--){
		scanf("%lld %lld",&a,&b);
		if(a == 0 || b == 0){
			printf("invalid\n");
			continue;
		}else{
			x1 = gcd(a,b); x2 = (a*b)/x1;
			printf("%lld %lld\n",x1,x2);
		}
	}
	return 0;
}
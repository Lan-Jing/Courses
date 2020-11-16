#include<stdio.h>

long long int N,M,sum[20010];
long long int c_sgn = 1,c_x;

int main(){
    scanf("%lld %lld",&N,&M);
    int i;
    for(i = 1;i <= N;i++){
        int temp;
        scanf("%d",&temp),sum[i] = sum[i-1]+temp;
    }

    for(i = 1;i <= M;i++){
        int temp; scanf("%d",&temp);
        if(temp == 1){
            c_sgn = -c_sgn,c_x = -c_x;
        }else if(temp == 2){
            scanf("%d",&temp); c_x += temp;
        }else{
            int l,r; scanf("%d %d",&l,&r);
            printf("%lld\n",c_sgn*(sum[r]-sum[l-1]) + (r-l+1)*c_x);
        }
    }
    return 0;
}

#include<stdio.h>
#include<string.h>

int n,temp,ans,a[101];
int l = 1,r;
int max(int a,int b){ return a > b ? a : b; }

int main(){
    scanf("%d",&n);
    int i;
    for(i = 1;i <= n;i++) scanf("%d",&a[i]);
    for(i = 1;i <= n;i++){
        temp += a[i];
        if(temp < 0) temp = 0,l = i+1;
        if(temp > ans){
            ans = temp;
            r = i;
        }
    }
    printf("%d\n",ans);
    for(i = l;i <= r;i++){
        if(i < r) printf("%d ",a[i]);
        else printf("%d\n",a[i]);
    }
    return 0;
}
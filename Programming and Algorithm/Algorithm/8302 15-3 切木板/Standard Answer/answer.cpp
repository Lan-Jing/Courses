#include<cstdio>
#include<iostream>
#include<algorithm>
#include<cstring>
#include<queue>
using namespace std;
typedef long long ll;
priority_queue<ll,vector<ll>,greater<ll> > a;//这里直接调用优先队列
#define in(t) freopen("t.in","r",stdin)
#define out(t) freopen("t.out","w",stdout)
#define m(a) memset(a,0,sizeof(a))
int main(){
    long long ans=0,n,t;
    scanf("%lld",&n);
    for(int i=1;i<=n;i++){
        scanf("%lld",&t);
        a.push(t);
    }
    for(int i=1;i<=n-1;i++){
        int c,d;
        c=a.top();
        a.pop();
        d=a.top();
        a.pop();//每次取最小的两个数
        ans+=c+d;//加上能量
        a.push(c+d);
    }
    printf("%lld",ans);

    return 0;

}
#include<iostream>
#include<cstdio> 
#include<cmath>
#include<map>
#include<algorithm>
using namespace std;
int n,c;
long long ans;
map<int,int> a;
int num[200005];
int main()
{
    scanf("%d%d",&n,&c);
    for(int i=1;i<=n;i++)
    {
        scanf("%d",&num[i]);
        a[num[i]]++;    //当前数的个数++ 
    }
    for(int i=1;i<=n;i++)
    {
        ans+=a[num[i]+c];    //答案+=相差为c的数的个数，即a[num[i]+c]位置的数的个数 
    }
    printf("%lld",ans);
    return 0;
}
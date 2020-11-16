#include<bits/stdc++.h>
using namespace std;
#define ll long long
#define il inline
ll m,a,b;
il ll gi()
{
    int a=0;char x=getchar();bool f=0;
    while((x<'0'||x>'9')&&x!='-')x=getchar();
    if(x=='-')x=getchar(),f=1;
    while(x>='0'&&x<='9')a=a*10+x-48,x=getchar();
    return f?-a:a;
}
ll c[100];
il void find(ll a,ll b)
{
    if(a<b)swap(a,b);
    if(a==b){printf("%lld\n",a);return;}
    int w=lower_bound(c,c+62,a)-c;
    find(b,a-c[w-1]);
}
int main()
{
    c[0]=1;c[1]=1;
    for(int i=2;i<=61;i++)c[i]=c[i-1]+c[i-2];     //printf("%lld\n",c[i]);
    a=gi(),b=gi();
    find(a,b);
    return 0;
}
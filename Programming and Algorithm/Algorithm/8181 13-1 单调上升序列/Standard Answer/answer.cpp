#include<iostream>
#include<cstdio>
#include<algorithm>
#define ll long long
#define rg register int
using namespace std;

int n;
int a[200005];
int f[200005];

int main(){
    cin>>n;
    for(rg i=1;i<=n;++i) cin>>a[i];
    rg ans=1; f[1]=a[1];
    for(rg i=2;i<=n;++i){
        rg l=1,r=ans,mid;
        while(l<=r){
            mid=(l+r)>>1;
            if(a[i]<=f[mid])r=mid-1;
            else l=mid+1;
        }f[l]=a[i];
        if(l>ans)++ans;
    }cout<<ans<<endl;
    return 0;
}
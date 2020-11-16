#include <iostream>
using namespace std;
#include <cstring>

int n,m,number[10000],sum[10000];

int main(){
    cin>>m>>n;
    for(int i = 1;i <= n;i++) cin>>number[i],sum[i] = sum[i-1] + number[i];
    int l = 0,r = 1;
    int ans = 0x3f3f3f3f;
    while(r <= n){
        if(sum[r] - sum[l] >= m){
            ans = min(ans,r-l);
            l++;
            if(l == r) r++;
        }else{
            r++;
        }
    }
    if(ans == 0x3f3f3f3f) cout<<0;
    else cout<<ans;
    return 0;
}
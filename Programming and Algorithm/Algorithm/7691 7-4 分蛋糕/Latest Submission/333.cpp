#include <iostream>
using namespace std;
#include <algorithm>
#include <string>
#include <time.h>

int n,m,food[55],temp[55],mouth[1030],ans;

bool check(int r){
    sort(mouth+1,mouth+1+m);
    for(int i = 1;i <= 1000;i++){
        random_shuffle(mouth+1,mouth+1+r);
        for(int j = 1;j <= n;j++) temp[j] = food[j];

        int flag = 1;
        for(int j = 1;j <= r;j++){
            int f = 1;
            for(int k = n;k >= 1;k--){
                if(temp[k] >= mouth[j]){
                    temp[k] -= mouth[j];
                    f = 0;
                    break;
                }
            }
            if(f){
                flag = 0;
                break;
            }
        }
        if(flag) return true;
    }
    return false;
}

int main(){
    srand(time(0));

    cin>>n;
    for(int i = 1;i <= n;i++) cin>>food[i];
    sort(food+1,food+1+n);

    cin>>m;
    for(int i = 1;i <= m;i++) cin>>mouth[i];

    int l = 0,r = m;
    while(l < r){
        int mid = (l+r)/2;
        if(check(mid)){
            l = mid + 1;
        }else r = mid;
    }
    cout<<l-1;
    return 0;
}

#include <iostream>
using namespace std;
#include <string>

string target; int length;
int cnt;
int main(){
    cin>>target; length = target.length();
    int ans = 0;
    for(int i = 0;i < length;i++)
        if(target[i] == ')') continue;
        else{
            cnt = 1;
            int tempAns = 0;
            for(int j = i + 1;j < length;j++){
                if(target[j] == '(') cnt++;
                else{
                    if(cnt == 0) break;
                    else{
                        cnt--;
                        tempAns += 2;
                        if(cnt == 0) ans = max(ans,tempAns);
                    }
                }
            }
        }
    cout<<ans;
    return 0;
}
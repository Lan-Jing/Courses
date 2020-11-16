#include <iostream>
using namespace std;
#include <stack>

int N,level[100010],direction[100010];
struct node{
    int level,carry;
    node(int level,int carry){ this->level = level, this->carry = carry; }
};
stack<node> stk;

int main(){
    cin>>N;
    int ans = N ;
    for(int i = 0;i < N;i++) cin>>level[i]>>direction[i];
    for(int i = 0;i < N;i++){
        if(!direction[i]){
            while(!stk.empty() && stk.top().level < level[i]){
                ans -= stk.top().carry, stk.pop();
            }
            if(!stk.empty()) ans--;
        }else{
            int flag = 0;
            while(!stk.empty() && stk.top().level < level[i]){
                flag += stk.top().carry;
                stk.pop();
            }
            stk.push(node(level[i],flag+1));
        }
    }
    cout<<ans<<endl;
    return 0;
}
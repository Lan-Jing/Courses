#include <iostream>
using namespace std;
#include <queue>

struct node{
    node(int pri,int index){ this->pri = pri, this->index = index; }
    bool operator < (const node& other) const { return this->pri < other.pri; }
    int pri,index;
};

int main(){
    int Ncase; cin>>Ncase;
    while(Ncase--){
        int n,m; cin>>n>>m;
        int result = 0;
        
        priority_queue <node> priQue;
        queue <node> que;
        
        for(int i = 0;i < n;i++){
            int temp; cin>>temp;
            priQue.push(node(temp,i));
            que.push(node(temp,i));
        }

//        cout<<priQue.top().pri<<endl;
//        cout<<priQue.size()<<endl;
        while(1){
            node top = que.front();
            que.pop();
            int flag = 0;
            if(top.pri == priQue.top().pri){
                result++;
                if(!priQue.empty()) priQue.pop();
                flag = 1;
            }else que.push(top);
            
            if(top.index == m && flag) break;
        }
        cout<<result<<endl;
    }
}
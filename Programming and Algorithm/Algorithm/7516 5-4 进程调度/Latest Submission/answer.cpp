#include <iostream>
using namespace std;
#include <queue>

struct node{
    int number,arriveTime,needTime,priority;
    node(int n = 0,int at = 0,int nt = 0,int p = 0){ number=n,arriveTime=at,needTime=nt,priority=p; }
    bool operator < (const node &other) const {
        if(this->priority == other.priority) return this->arriveTime > other.arriveTime;
        else return this->priority < other.priority;
    }
}list[100010]; int N;
priority_queue <node> que;

int main(){
    while(~scanf("%d%d%d%d",&list[N].number,&list[N].arriveTime,&list[N].needTime,&list[N].priority)){
//    for(int i = 1;i <= 10;i++){
//        scanf("%d%d%d%d",&list[N].number,&list[N].arriveTime,&list[N].needTime,&list[N].priority);
        N++;
    }

    int nowP = 0,nowTime = 0; node nowNode;
    int cflag = 0;
    while(1){
        nowTime++;
        while(nowP < N && list[nowP].arriveTime <= nowTime)
            que.push(list[nowP++]);

        if(nowNode.number) nowNode.needTime--; 
        if(!nowNode.needTime && nowNode.number){
            if(cflag) cout<<endl<<nowNode.number<<' '<<nowTime;
            else cflag = 1,cout<<nowNode.number<<' '<<nowTime;
            nowNode.number = 0;
            if(que.empty() && nowP == N) break;
        }

        if(!nowNode.number && !que.empty()){
            nowNode = que.top(); que.pop();
        }else if(nowNode.number && !que.empty() && que.top().priority > nowNode.priority){
            que.push(nowNode);
            nowNode = que.top(); que.pop(); 
        }
    }
    return 0;
}
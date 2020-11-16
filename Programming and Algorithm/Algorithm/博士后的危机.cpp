#include <iostream>
using namespace std;
#include <queue>
#include <cstring>

int n,con[2010][2010],dis[2010],vis[2010];
struct edge{
    int v, l;
    edge(int v, int l){ this->v = v, this->l = l; }
    bool operator < (const edge &other) const {
        return this->l < other.l;
    }
};
void dijkstra(int begin){
    dis[begin] = 0x3f3f3f3f;
    priority_queue<edge> que;
    que.push(edge(begin,0));

    while(!que.empty()){
        edge now = que.top(); que.pop();
        if(vis[now.v]) continue;
        else vis[now.v] = 1;

        for(int i = 1;i <= n;i++)
            if(con[now.v][i]){
                dis[i] = max(dis[i],min(dis[now.v],con[now.v][i]));
                que.push(edge(i,dis[i]));
            }
    }
}

int main(){
    cin>>n;
    int A,B,R;
    while(cin>>A>>B>>R){
        if(!A && !B && !R) break;
        con[A][B] = R;
    }
    dijkstra(1);
    for(int i = 2;i <= n;i++) cout<<dis[i]<<endl;
    return 0;
}
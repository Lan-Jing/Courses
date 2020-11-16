#include <iostream>
using namespace std;
#include <cstring>
#include <queue>

int n;
int cost[1010][1010],vis[1010];
struct node{
	int index,dis;
	node(int a,int b){ this->index = a,this->dis = b; }
	bool operator < (const node &other) const {
		return other.dis < this->dis;
	}
};

void dijkstra(int begin){
	priority_queue<node> que;
	que.push(node(begin,0));
	
	while(!que.empty()){
		node now = que.top(); que.pop();
		if(vis[now.index]) continue;
		else vis[now.index] = 1;
		
		for(int i = now.index + 1;i <= n;i++){
			cost[begin][i] = min(cost[begin][i],cost[begin][now.index] + cost[now.index][i]);
			que.push(node(i,cost[begin][i]));
		}
	}
}

int main(){
	cin>>n;
	memset(cost,0x3f,sizeof(cost)); 

	for(int i = 1;i < n;i++)
		for(int j = 1;j <= n-i;j++)
			cin>>cost[i][i+j];
	dijkstra(1);
	cout<<cost[1][n];
	return 0;
}
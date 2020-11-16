#include <iostream>
using namespace std;
#include <cstring>

int N,M;
int con[1010][1010];
int ans,vis[1010];

void dfs(int now){
	vis[now] = 1;
	ans = max(ans,now);
	for(int i = 1;i <= N;i++){
		if(!con[now][i] || vis[i]) continue;
		dfs(i);
	}
}

int main(){
	cin>>N>>M;
	for(int i = 1;i <= M;i++){
		int a,b; cin>>a>>b;
		con[a][b] = 1;
	}
	for(int i = 1;i <= N;i++){
		ans = 0;
		memset(vis,0,sizeof(vis));
		
		dfs(i);
		
		cout<<ans<<' ';
	}
	return 0;
}
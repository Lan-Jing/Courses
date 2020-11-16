#include <iostream>
using namespace std;
#include <cstring>

int n,m,ans;
int con[1010][1010],indegree[1010],outdegree[1010];
int dp[1010];

int dfs(int now){
	if(dp[now]) return dp[now];
	
	int tempAns = 0;
	for(int i = 1;i <= n;i++) 
		if(con[now][i])
			tempAns += dfs(i);
	dp[now] = tempAns;
	return tempAns;
}

int main(){
	cin>>n>>m;
	for(int i = 1;i <= m;i++){
		int a,b; cin>>a>>b;
		con[b][a] = 1;
		indegree[a]++; outdegree[b]++;
	}
	for(int i = 1;i <= n;i++)
		if(!outdegree[i]) dp[i] = 1;
//	for(int i = 1;i <= n;i++) cout<<dp[i]<<' '; cout<<endl;
	for(int i = 1;i <= n;i++)
		if(!indegree[i])
			dfs(i);
	for(int i = 1;i <= n;i++)
		if(!indegree[i])
			ans += dp[i];
//	for(int i = 1;i <= n;i++) cout<<dp[i]<<' '; cout<<endl;
	cout<<ans;
	return 0;
}
#include <iostream>
using namespace std;
#include <cstring>

int n,m,ans;
int con[1010][1010],degree[1010];

void dfs(int now){
	for(int i = 1;i <= n;i++)
		if(con[now][i]){
			con[now][i] = con[i][now] = 0;
			degree[now]--; degree[i]--;
			
			dfs(i);
			break;
		}
}

int main(){
	cin>>n>>m;
	for(int i = 1;i <= m;i++){
		int a,b; cin>>a>>b;
		con[a][b] = con[b][a] = 1;
		degree[a]++; degree[b]++;
	}  
	for(int i = 1;i <= n;i++){
		if(!degree[i]) continue;
		
		ans++;
		dfs(i);
	}
	cout<<ans;
	return 0;
}
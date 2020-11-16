#include <iostream>
using namespace std;
#include <string>

int mat[105][105];
int n,m;
int mx[8]={-1,0,1,-1,1,-1,0,1};
int my[8]={-1,-1,-1,0,0,1,1,1};

int ans;
void dfs(int y,int x){
	mat[y][x] = 0;
	for(int i = 0;i < 8;i++){
		int nx = x + mx[i];
		int ny = y + my[i];
		if(1 <= nx && nx <= m && 1 <= ny && ny <= n && mat[ny][nx])
			dfs(ny,nx);
	}
}

int main(){
	cin>>n>>m;
	for(int i = 1;i <= n;i++)
		for(int j = 1;j <= m;j++)
			cin>>mat[i][j];
	for(int i = 1;i <= n;i++)
		for(int j = 1;j <= m;j++)
			if(mat[i][j]){
				ans++;
				dfs(i,j);
			}
			
	cout<<ans;
	return 0;
}
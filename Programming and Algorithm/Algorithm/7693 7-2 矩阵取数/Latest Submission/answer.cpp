#include <iostream>
using namespace std;
#include <cstring>

int matrix[50][50],flood[50][50],used[50][50],old[50][50];
int n,m,ans;
int ry[9]={-1,0,1,-1,0,1,-1,0,1};
int rx[9]={-1,-1,-1,0,0,0,1,1,1};

int rry[4]={-1,0,1,0};
int rrx[4]={0,-1,0,1};

int check(int y,int x){
	for(int i = 0;i < 9;i++)
		if(1 <= y+ry[i] && y+ry[i] <= n && 1 <= x+rx[i] && x+rx[i] <= m)
			if(flood[y+ry[i]][x+rx[i]] == 1) return false;
	return true;
}

void dfs(int ny,int nx,int temp){
	used[ny][nx] = 1;
	ans = max(ans,temp);
//	cout<<ny<<' '<<nx<<' '<<temp<<endl;
	
	int x,y;
	if(check(ny,nx)){
		flood[ny][nx] = 1;
		
		for(int i = 0;i < 4;i++){
			x = nx + rrx[i];
			y = ny + rry[i];
			if(1 <= x && x <= m && 1 <= y && y <= n && used[y][x] == 0)
				dfs(y,x,temp + matrix[ny][nx]);
		}
		flood[ny][nx] = 0;
	}
	
	for(int i = 0;i < 4;i++){
		x = nx + rrx[i];
		y = ny + rry[i];
		if(1 <= x && x <= m && 1 <= y && y <= n && used[y][x] == 0)
			dfs(y,x,temp);
	}
}

int main(){
	cin>>n>>m;
	for(int i = 1;i <= n;i++)
		for(int j = 1;j <= m;j++) cin>>matrix[i][j];
	
	for(int i = 1;i <= n;i++)
		for(int j = 1;j <= m;j++){
			memset(flood,0,sizeof(flood));
			memset(used,0,sizeof(used));
			dfs(i,j,0);
		}
	cout<<ans;
	return 0;
}
#include <iostream>
using namespace std;

int N,matrix[10][10];
int dp[20][20][20][20],ry[2]={-1,0},rx[2]={0,-1};

int main(){
	cin>>N;
	int x,y,value;
	while(1){
		cin>>y>>x>>value;
		if(!x && !y && !value) break;
		matrix[y][x] = value;
	}
/*	
	cout<<endl;
	for(int i = 1;i <= N;i++){
		for(int j = 1;j <= N;j++) cout<<matrix[i][j]<<' '; cout<<endl;
	}
*/	
	for(int y1 = 1;y1 <= N;y1++)
		for(int x1 = 1;x1 <= N;x1++)
			for(int y2 = 1;y2 <= N;y2++)
				for(int x2 = 1;x2 <= N;x2++){
					int temp = 0;
					int flag = (y1 == y2 && x1 == x2);
					for(int r1 = 0;r1 < 2;r1++)
						for(int r2 = 0;r2 < 2;r2++){
							int py1 = y1 + ry[r1], px1 = x1 + rx[r1];
							int py2 = y2 + ry[r2], px2 = x2 + rx[r2];
							
							temp = dp[py1][px1][py2][px2] + matrix[y1][x1] + matrix[y2][x2] - flag*matrix[y1][x1];
							dp[y1][x1][y2][x2] = max(dp[y1][x1][y2][x2],temp);
						}
				}
	cout<<dp[N][N][N][N];
	return 0;
}
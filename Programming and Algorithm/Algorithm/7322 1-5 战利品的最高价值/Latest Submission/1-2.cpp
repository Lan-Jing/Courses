#include <iostream>
using namespace std;

int grid[4][4];
int dp[4][4];

int main(){
	for(int i = 0;i < 4;i++)
		for(int j = 0;j < 4;j++){
			cin>>grid[i][j];
			dp[i][j] = -0x3f3f3f3f;
		}
	dp[0][0] = grid[0][0];
	for(int i = 0;i < 4;i++)
		for(int j = 0;j < 4;j++)
			if(i || j){
				if(i >= 1) dp[i][j] = max(dp[i][j],dp[i-1][j] + grid[i][j]);
				if(j >= 1) dp[i][j] = max(dp[i][j],dp[i][j-1] + grid[i][j]);
			}
	cout<<dp[3][3];
	return 0;
}
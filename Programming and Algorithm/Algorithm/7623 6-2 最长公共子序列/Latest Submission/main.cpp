#include <iostream>
using namespace std;
#include <string>

string a,b; int la,lb; 
int dp[1010][1010];
int main(){
	cin>>a>>b; la = a.length(),lb = b.length();
	a = " " + a,b = " " + b;
	for(int i = 1;i <= la;i++)
		for(int j = 1;j <= lb;j++){
			if(a[i] == b[j]){
				dp[i][j] = dp[i-1][j-1] + 1;
			}else{
				dp[i][j] = max(dp[i-1][j],dp[i][j-1]);
			}
		}
	cout<<dp[la][lb];
	return 0;
}
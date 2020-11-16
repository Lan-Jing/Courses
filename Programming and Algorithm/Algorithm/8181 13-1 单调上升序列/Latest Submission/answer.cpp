#include <iostream>
using namespace std;

int n,num[10010];
int dp[10010],ans;

int main(){
	cin>>n;
	for(int i = 1;i <= n;i++) cin>>num[i];
	for(int i = 1;i <= n;i++) dp[i] = 1;
	
	for(int i = 2;i <= n;i++){
		for(int j = 1;j <= i;j++){
			if(num[j] < num[i])
				dp[i] = max(dp[i],dp[j]+1);
		}
//		cout<<dp[i]<<endl;
		ans = max(dp[i],ans);
	}
	cout<<ans;
	return 0;
}
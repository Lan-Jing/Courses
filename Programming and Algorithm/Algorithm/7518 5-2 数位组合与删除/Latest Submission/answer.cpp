#include <iostream>
using namespace std;
#include <queue>
#include <algorithm>

int n,m,list[10010],mark[10010],ptr;
priority_queue <int,vector<int>,greater<int> > que;

int main(){
	cin>>n>>m;
	que.push(1);
	while(que.size() < 2*n){
		int now = que.top(); que.pop(); list[ptr++] = now; 
		que.push(2*now+1), que.push(4*now+5);
	}
	while(!que.empty()){
		long long int now = que.top(); que.pop(); list[ptr++] = now;
	}
	string ans;
	for(int i = 0;i < n;i++){
		long long int temp = list[i];
		string ttemp;
		while(temp){
			ttemp += temp%10 + '0'; temp /= 10;
		}
		reverse(ttemp.begin(),ttemp.end());
		ans += ttemp;
	}
	cout<<ans<<endl;
	int ll = ans.length(),l = 0,r = 1;
	while(m && r < ll){
		while(m && l >= 0 && ans[l] < ans[r]){
			mark[l] = 1;
			m--;
			while(l >= 0 && mark[l]) l--;
		}
		l = r,r++;
	}
	r = l-1;
	while(m && r >= 0){
		if(!mark[r]){
			mark[r] = 1; m--;
		}
		r--;
	}
	for(int i = 0;i < ll;i++)
		if(!mark[i]) cout<<ans[i]; cout<<endl;
	return 0;
}
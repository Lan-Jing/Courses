#include <iostream>
using namespace std;
#include <string>
#include <cstring>
#include <algorithm>
#include <queue>

priority_queue<int, vector<int>, greater<int> > que;
int n,ans,num[100010];

int main(){
	cin>>n;
	for(int i = 1;i <= n;i++) cin>>num[i],que.push(num[i]);
	for(int i = 1;i < n;i++){
		int a = que.top(); que.pop();
		int b = que.top(); que.pop();
//		cout<<a<<' '<<b<<endl;
		ans += a+b;
		que.push(a + b);
	}
	cout<<ans;
	return 0;
}
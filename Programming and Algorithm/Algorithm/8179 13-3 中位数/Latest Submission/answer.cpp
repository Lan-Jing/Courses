#include <iostream>
using namespace std;
#include <cstring>
#include <queue>

priority_queue<int,vector<int>, greater<int> > greatQ;
priority_queue<int,vector<int>, less<int> > lowerQ;

int n,num[100010];

int main(){
	cin>>n;
	for(int i = 1;i <= n;i++) cin>>num[i];
	
	for(int i = 1;i <= n;i += 2){
		for(int j = max(1,i-1);j <= i;j++){
			greatQ.push(num[j]);
			if(greatQ.size() > (i-1)/2){
				int temp = greatQ.top(); greatQ.pop();
				lowerQ.push(temp);
			}
		}
		cout<<lowerQ.top()<<endl;
	}
	return 0;
}
#include <iostream>
using namespace std;

int n,d;
int point[100010],rightMost[100010];

int main(){
	cin>>n>>d;
	for(int i = 0;i < n;i++) cin>>point[i];
	int j = 0;
	for(int i = 0;i < n;i++){
		while(j+1 < n && point[j+1]-point[i] <= d) j++;
		rightMost[i] = j;
	}
//	for(int i = 0;i < n;i++) cout<<rightMost[i]<<' '; cout<<endl;
	
	long long int ans = 0;
	for(int i = 0;i < n-2;i++){
		int temp = rightMost[i] - i;
		ans += temp*(temp-1)/2;
	}
	cout<<ans<<endl;
	return 0;
}
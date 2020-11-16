#include <iostream>
using namespace std;
#include <string>

int m,n;
int flower[100010];
int cnt;
int l;

int main(){
	cin>>m;
	flower[m] = 1;
	for(int i = 0;i <= m;i++) {
		if(i < m) cin>>flower[i];
		if(flower[i]){
			if(i - l >= 4){
				cnt += (i-l-2)/2;
			}
			l = i;
		}
	}
	cin>>n;
	if(cnt >= n) cout<<1;
	else cout<<0;
	return 0;
}
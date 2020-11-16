#include <iostream>
using namespace std;
#include <algorithm>

int n;
struct node{
	int index,num;	
	bool operator < (const node &other) const {
		return this->num < other.num;
	}
}num[100010];

int main(){
	cin>>n;
	for(int i = 1;i <= n;i++){
		cin>>num[i].num;
		num[i].index = i;
	}
	sort(num+1,num+1+n);
	
	if(num[n].num >= num[n-1].num * 2) cout<<num[n].index-1;
	else cout<<-1;
	
	return 0;
}
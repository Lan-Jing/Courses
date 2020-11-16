#include <iostream>
using namespace std;
#include <string>
#include <algorithm>

int n;
string num[100010];

bool cmp(string a, string b){
	return a + b > b + a;
	return 0;
}

int main(){
	cin>>n;
	for(int i = 1;i <= n;i++) cin>>num[i];
	sort(num+1,num+1+n,cmp);
	for(int i = 1;i <= n;i++) cout<<num[i];
	return 0;
}
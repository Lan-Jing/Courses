#include <iostream>
using namespace std;
#include <algorithm>
#include <string>

int n,add,num[100010];

int main(){
	cin>>n>>add;
	for(int i = 1;i <= n;i++) cin>>num[i];
	for(int i = 1;i <= add;i++)
		next_permutation(num+1,num+1+n);
	for(int i = 1;i <= n;i++) cout<<num[i]<<' '; 
	return 0;
}
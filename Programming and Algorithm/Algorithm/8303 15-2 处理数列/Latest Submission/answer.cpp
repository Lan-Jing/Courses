#include <iostream>
using namespace std;
#include <string>
#include <cstring>
#include <set>

int n,num[100010];

int main(){
	set<int> Set;
	cin>>n;
	for(int i = 1;i <= n;i++){
		cin>>num[i];
		Set.insert(num[i]);
	}
	cout<<Set.size()<<endl;
	for(set<int>::iterator iter = Set.begin();iter != Set.end();iter++)
		cout<<*iter<<' ';
	return 0;
}
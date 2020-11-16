#include <iostream>
using namespace std;
#include <set>

set<int> Set;
int list[100010],N,ans;

int main(){
	while(~scanf("%d",&list[N])){
		if(Set.count(list[N]) == 0){
			ans++;
			Set.insert(list[N]);
		}
		N++;
	}
	cout<<ans;
	return 0;
}
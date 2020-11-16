#include <iostream>
using namespace std;
#include <set>
#include <string>

set<string> Set;
set<string> ans;
string target;

int main(){
	cin>>target;
	int l = target.length();
	int flag = 0;
	
	for(int i = 9;i < l;i++){
		int j = i - 9;
		string temp = "";
		for(int t = j;t <= i;t++) temp += target[t];
		if(Set.count(temp))
			ans.insert(temp);
		Set.insert(temp);
	}
	for(set<string>::iterator iter = ans.begin();iter != ans.end();iter++){
		if(!flag){
			flag = 1;
			cout<<*iter;
		}else cout<<endl<<*iter;
	}
	return 0;
}
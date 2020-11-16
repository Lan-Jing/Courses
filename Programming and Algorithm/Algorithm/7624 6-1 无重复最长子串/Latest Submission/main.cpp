#include <iostream>
using namespace std;
#include <set>

set<char> alph; 
string target;
int ans,lt,l,r;

int main(){
	cin>>target; lt = target.length();
	ans = 1;
	alph.insert(target[0]);
	while(1){
		if(r == lt - 1) break;
		if(!alph.count(target[r+1])){
			alph.insert(target[++r]);
			ans = max(ans,r-l+1);	
		}else if(alph.count(target[l])){
			alph.count(target[l++]);
		}else l++;
		if(l > r) r = l;
	} 
	cout<<ans;
	return 0;
}
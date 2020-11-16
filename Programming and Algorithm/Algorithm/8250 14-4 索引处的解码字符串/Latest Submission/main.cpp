#include <iostream>
using namespace std;
#include <string>

string target;
string ans;
int k;

int main(){
	cin>>target;
	int l = target.length();
	cin>>k;
	
	for(int i = 0;i < l;i++){
		if('0' <= target[i] && target[i] <= '9'){
			string tempAns = ans;
			for(int j = 1; j <= target[i]-'0' - 1;j++){
				ans += tempAns;
				if(ans.length() > k){
					cout<<ans[k-1];
					return 0;
				}
			}
//			cout<<ans<<endl;
		}else{
			ans += target[i];
		}
	}
	cout<<ans[k-1];
	return 0;
}
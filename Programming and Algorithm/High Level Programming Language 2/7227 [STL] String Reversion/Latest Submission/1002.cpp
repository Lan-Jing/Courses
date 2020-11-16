#include <iostream>
using namespace std;
#include <string>
#include <algorithm>

int main(){
	int t; cin>>t;
	while(t--){
		string target; cin>>target;
		int length = target.length();
		int first = 0,last = 0;
		
		for(int i = 0;i < length;i++){
			if(target[i] == '_' || (i == length - 1 && target[i] != '_')){
				last = target[i] == '_' ? i : i+1;
				reverse(target.begin()+first,target.begin()+last);
				first = i + 1;
			}
		}
		cout<<target<<endl;
	}
	return 0;
}
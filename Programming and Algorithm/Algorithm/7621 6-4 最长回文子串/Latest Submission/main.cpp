#include <iostream>
using namespace std;
#include <string>

string target,ans; int lt,maxL;

int main(){
	cin>>target; lt = target.length();
	
	for(int i = 0;i < lt;i++){
		int j = 0,tempL = 0;
		while(i - j >= 0 && i + j < lt){
			if(target[i-j] != target[i+j]) break;
			else{
				tempL = 2*j+1;
				if(i - j - 1 >= 0 && i + j + 1 < lt) j++;
				else break;
			}
		}
		if(tempL > maxL){
			maxL = tempL;
			ans.clear();
			for(int k = i - j + 1;k <= i + j - 1;k++) ans += target[k];
		}
		
		j = 0,tempL = 0;
		while(i-j >= 0 && i + j + 1 < lt){
			if(target[i-j] != target[i+j+1]) break;
			else{
				tempL = 2*(j+1);
				if(i - j - 1 >= 0 && i + j + 2 < lt) j++;
				else break;
			}
		}
		if(tempL > maxL){
			maxL = tempL;
			ans.clear();
			for(int k = i - j + 1;k <= i + j;k++) ans += target[k];
		}
//		cout<<maxL<<' '<<ans<<endl;
	}
	cout<<ans;
	return 0;
}

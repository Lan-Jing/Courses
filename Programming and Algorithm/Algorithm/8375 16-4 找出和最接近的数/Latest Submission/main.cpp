#include <iostream>
using namespace std;
#include <cstring>
#include <string>
#include <algorithm>

int n,target,num[100010];
int minError = 0x3f3f3f3f;
int error;

int main(){
	cin>>n;
	for(int i = 1;i <= n;i++) cin>>num[i];
	cin>>target;
	sort(num+1,num+1+n);
	for(int i = 1;i <= n-2;i++)
		for(int j = i + 1;j <= n-1;j++)
			for(int k = j + 1;k <= n;k++){
				if(abs(num[i] + num[j] + num[k] - target) < minError){
					minError = abs(num[i]+num[j]+num[k]-target);
					error = num[i]+num[j]+num[k]-target;
				}
					
			}
	cout<<target + error;
	return 0;
}
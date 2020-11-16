#include <iostream>
using namespace std;
#include <set>
#include <cstring>

int n,have[20];

int main(){
	cin>>n;
	for(int i = 0;i < 10;i++){
		int temp; cin>>temp;
		have[temp-n] = 1;
	}	
	for(int i = 0;i <= 10;i++)
		if(!have[i]) cout<<i+n;
	return 0;
}
#include <iostream>
using namespace std;
#include <cstring>

int t,n,m;
int out[110];

int main(){
	cin>>t;
	while(t--){
		cin>>n>>m;
		memset(out,0,sizeof(out));
		
		int ptr = 1,count = 0,tempN = n;
		while(tempN){
			if(!out[ptr]){
				count++;
				if(count == m){
					count = 0;
					out[ptr] = 1;
					
					tempN--;
					if(!tempN) cout<<ptr<<endl;
				}
			}
			
			ptr = ptr == n ? 1 : ptr + 1;
		}
	}
	return 0;
}

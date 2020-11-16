#include <iostream>
using namespace std;
#include <cstring>
#include <set>

int a,b;
long long int fib[100];
set<int> S;

int main(){
	cin>>a>>b;
	fib[1] = fib[2] = 1;
	for(int i = 3;i <= 90;i++){
		fib[i] = fib[i-1] + fib[i-2];
//		cout<<fib[i]<<endl;
	}
	
	int i = 90;
	while(a){
		S.insert(a);
		while(fib[i] >= a && i > 1) i--;
		a -= fib[i];
//		cout<<a<<endl;
	}
	i = 90;
	while(b){
		if(S.count(b)){
			cout<<b;
			break;
		}
		while(fib[i] >= b && i > 1) i--;
		b -= fib[i];
//		cout<<b<<endl;
	}
	return 0;
}
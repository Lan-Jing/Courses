#include <iostream>
using namespace std;
#include <cmath>

int list[100010];
int K,n;

int main(){
	cin>>K>>n;
	for(int i = 1;i <= K;i++) cin>>list[i];
	int countBeforeN = pow(2,n-1) - 1;
	int total = min(K,int(pow(2,n) - 1));
	for(int i = countBeforeN + 1;i <= total;i++) cout<<list[i]<<' ';
	return 0;
}
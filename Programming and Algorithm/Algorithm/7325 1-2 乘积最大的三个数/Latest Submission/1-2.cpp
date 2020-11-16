#include <iostream>
using namespace std;
#include <algorithm>

int number[10];
int answer = -0x3f3f3f3f;

int main(){
	for(int i = 0;i < 10;i++) cin>>number[i];
	for(int i = 0;i < 10;i++)
		for(int j = 0;j < 10;j++)
			for(int k = 0;k < 10;k++)
				if(i != j && i != k && j != k){
					int temp = number[i]*number[j]*number[k];
					answer = max(answer,temp);
				}
	cout<<answer;
	return 0;
}
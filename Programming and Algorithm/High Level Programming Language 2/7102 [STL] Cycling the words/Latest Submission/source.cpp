#include <iostream>
using namespace std;
#include <deque>

deque<string> que;

int main(){
	string temp;
	while(cin>>temp) que.push_back(temp);
	int Count = que.size();
//	cout<<Count<<endl;
	for(int i = 0;i < Count;i++){
		int tempCount = 0;
		for(deque<string>::iterator iter = que.begin();iter != que.end();iter++){
			tempCount++;
			if(tempCount == Count) cout<<(*iter)<<endl;
			else cout<<(*iter)<<' ';
		}
		que.push_back(que.front());
		que.pop_front();
	}
	return 0;
}
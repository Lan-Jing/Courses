#include <iostream>
using namespace std;
#include <set>

string target;
set<char> Set;
int answer;

int main(){
	cin>>target;
	int length = target.length();
	for(int start = 0;start < length;start++){
		Set.clear();
		int tempAns = 0;
		for(int ptr = start;ptr < length;ptr++){
			if(Set.count(target[ptr])){
				answer = max(answer,tempAns);
				break;
			}else{
				tempAns++;
				Set.insert(target[ptr]);
			}
		}
	}
	cout<<answer;
	return 0;
}
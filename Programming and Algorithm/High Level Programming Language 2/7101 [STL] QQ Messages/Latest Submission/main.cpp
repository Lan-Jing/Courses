#include <iostream>
using namespace std;
#include <list>
#include <utility>

list <pair<string,string> > que;
int Count;

int main(){
	cin>>Count;
	for(int i = 0;i < Count;i++){
		string ID, Message;
		cin>>ID>>Message;
		pair<string,string> temp_pair = make_pair(ID,Message);

		for(list< pair<string,string> >::iterator iter = que.begin();iter != que.end();iter++)
			if((*iter).first == temp_pair.first){
				que.erase(iter);
				break;
			}
//		cout<<"hello"<<endl;
		que.push_front(temp_pair);
	}
	for(list< pair<string,string> >::iterator iter = que.begin();iter != que.end();iter++)
		cout<<(*iter).first<<":\t"<<(*iter).second<<endl;
	return 0;
}